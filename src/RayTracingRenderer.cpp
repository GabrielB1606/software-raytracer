#include "RayTracingRenderer.h"

RayTracingRenderer::RayTracingRenderer(size_t width, size_t height){
    this->width = width;
    this->height = height;
    this->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
    this->textureID = nullptr;

    this->aspectRatio = width/height;

    this->cam.position = glm::vec3(0.f, 0.f, 1.f);
    this->cam.z_direction = -1.f;

}

RayTracingRenderer::~RayTracingRenderer(){
    SDL_FreeSurface(surface);
    if(this->textureID != nullptr){
        SDL_DestroyTexture((SDL_Texture*)textureID);
        this->textureID = nullptr;
    }

    for( Shape* s : scene )
        delete[] s;
    
    for( DirectionalLight* l:lights )
        delete[] l;
}

void RayTracingRenderer::changeRenderResolution(size_t width, size_t height){

    SDL_FreeSurface(surface);
    this->width = width;
    this->height = height;
    this->aspectRatio = width/height;
    this->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);

}

void RayTracingRenderer::putPixel(glm::vec2 position, glm::vec3 color){

    // Access pixel data and update framebuffer
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);
    int x = position.x;
    int y = position.y;
    // Write pixel data to framebuffer
    Uint32 color_num = 0xff00ffff /* calculate or retrieve pixel color */;
    pixels[y * pitch + x] = color_num;

}

glm::vec4 RayTracingRenderer::fragmentFunction(glm::vec2 coord){
    float alpha = 1.f;
    Ray ray =  this->cam.getRay(coord);

    glm::vec3 fragColor = glm::vec3(0.f);

    bool everHitted = false;
    float multiplier = 1.f;

    for (size_t i = 0; i < bounces; i++){
        TracingInfo info = traceRay(ray);
        
        if( info.hittedShape == nullptr ){
            fragColor +=  multiplier*clearColor;
            break;
        }

        everHitted = true;

        glm::vec3 normal = info.hittedShape->normalAt(info.hitPosition);

        

        Ray ray2light;
        ray2light.origin = info.hitPosition + normal*0.0001f;

        for(DirectionalLight* l:lights){

            ray2light.direction = l->direction;
            float shadowComponent = 1.f;

            if( activateShadow && traceRay(ray2light).hittedShape != nullptr ){
                shadowComponent = 0.15f;
            }

            switch (l->model){
            case PHONG:
                fragColor += shadowComponent*multiplier*l->phong(info.hittedShape, normal, ray.direction);
                break;
            
            case OREN_NAYAR:
                fragColor += shadowComponent*multiplier*l->oren_nayar(info.hittedShape, normal, ray.direction);
                break;
            
            case COOK_TORRANCE:
                fragColor += shadowComponent*multiplier*l->cook_torrance(info.hittedShape, normal, ray.direction);
                break;

            default:
                break;
            }
            
        }

        multiplier *= (1.f-info.hittedShape->getRoughness()) * 0.5f;

        if( info.hittedShape->isRefractive() ){
            ray.direction = glm::refract(ray.direction, normal, info.hittedShape->getETA() );
            ray.origin = info.hitPosition + ray.direction*0.0001f;
        }else{
            ray.origin = info.hitPosition + normal*0.0001f;
            ray.direction = glm::reflect(ray.direction, normal);
        }

    } 

    if(everHitted){
        glm::vec3 ambient = glm::vec3(1.f);
        
        for(DirectionalLight* l:lights)
            ambient *= l->color;
        
        return glm::vec4( this->ambientIntensity*ambient +fragColor, alpha);
    }

    
    return glm::vec4( this->clearColor,  alpha);
    
}

RayTracingRenderer::TracingInfo RayTracingRenderer::traceRay(Ray r){

    TracingInfo info;

    glm::vec3 originalPos = r.origin;
    Shape* closestShape = nullptr;
    float closestHit = FLT_MAX;
    float t;
    for(Shape* shape:this->scene){
        
        r.origin = originalPos - shape->getPosition();
        t = shape->hit(r);

        if(t>=0.f && t<closestHit){
            closestShape = shape;
            closestHit = t;
        }

    }

    if(closestShape == nullptr)
        info.hittedShape = nullptr;
    else{
        r.origin = originalPos;
        info.hitDistance = closestHit;
        info.hittedShape = closestShape;
        info.hitPosition = r.at(closestHit);
    }

    return info;
}

ImTextureID RayTracingRenderer::render(SDL_Renderer* renderer){

    if(this->textureID != nullptr){
        SDL_DestroyTexture((SDL_Texture*)textureID);
        this->textureID = nullptr;
    }

    // Lock the surface to access its pixel data
    if (SDL_LockSurface(surface) != 0) {
        // Handle error
        std::cerr << "surface did not lock\n";
        return nullptr;
    }

    glm::vec2 screenCoord;
    Uint32 color;

    // Access pixel data and update framebuffer
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            // Write pixel data to framebuffer
            screenCoord = glm::vec2((float)x/width,(float)y/height);
            screenCoord.x = (screenCoord.x*2) -1;
            screenCoord.y = (screenCoord.y*2) -1;

            color = vec4ToARGB( fragmentFunction( screenCoord ));
            pixels[y * pitch + x] = color;
        }
    }

    // Unlock the surface to make it visible
    SDL_UnlockSurface(surface);

    if(saveFrame){
        SDL_SaveBMP(surface, "screenshot.bmp");
        saveFrame = false;
    }

    this->textureID = (ImTextureID)(intptr_t)SDL_CreateTextureFromSurface(renderer, surface);

    return this->textureID;

}

ImTextureID RayTracingRenderer::getLastFrame(){
    return this->textureID;
}

void RayTracingRenderer::takeScreenshot(){
    if(realtime)
        this->saveFrame = true;
    else
        SDL_SaveBMP(surface, "screenshot.bmp");
}

void RayTracingRenderer::addShape(Shape *shape){
    this->scene.push_back(shape);
}

void RayTracingRenderer::removeShape(size_t index){
    delete[] scene[index];
    scene.erase( scene.begin()+index );
}

Shape *RayTracingRenderer::getShape(size_t index){
    return this->scene[index];
}

size_t RayTracingRenderer::getSceneSize(){
    return this->scene.size();
}

void RayTracingRenderer::addLight(DirectionalLight *light){
    lights.push_back(light);
}

void RayTracingRenderer::removeLight(size_t index){
    delete[] lights[index];
    lights.erase(lights.begin()+index);
}

DirectionalLight *RayTracingRenderer::getLight(size_t index){
    return lights[index];
}

size_t RayTracingRenderer::getLightsSize(){
    return lights.size();
}

Camera *RayTracingRenderer::getCamRef(){
    return &this->cam;
}

glm::vec3 RayTracingRenderer::getClearColor(){
    return this->clearColor;
}

void RayTracingRenderer::setClearColor(glm::vec3 color){
    this->clearColor = color;
}

uint32_t RayTracingRenderer::vec3ToARGB(const glm::vec3 &color){
    uint32_t a = 255 & 0xFF;
    uint32_t r = static_cast<uint32_t>(__min(__max(color.r, 0.f), 1.f) * 255) & 0xFF;
    uint32_t g = static_cast<uint32_t>(__min(__max(color.g, 0.f), 1.f) * 255) & 0xFF;
    uint32_t b = static_cast<uint32_t>(__min(__max(color.b, 0.f), 1.f) * 255) & 0xFF;

    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t RayTracingRenderer::vec4ToARGB(const glm::vec4 &color){
    uint32_t a = static_cast<uint32_t>(__min(__max(color.a, 0.f), 1.f) * 255) & 0xFF;
    uint32_t r = static_cast<uint32_t>(__min(__max(color.r, 0.f), 1.f) * 255) & 0xFF;
    uint32_t g = static_cast<uint32_t>(__min(__max(color.g, 0.f), 1.f) * 255) & 0xFF;
    uint32_t b = static_cast<uint32_t>(__min(__max(color.b, 0.f), 1.f) * 255) & 0xFF;

    return (a << 24) | (r << 16) | (g << 8) | b;
}