#include "RayTracingRenderer.h"

RayTracingRenderer::RayTracingRenderer(size_t width, size_t height){
    this->width = width;
    this->height = height;
    this->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
    this->textureID = nullptr;
}

RayTracingRenderer::~RayTracingRenderer(){
    SDL_FreeSurface(surface);
    if(this->textureID != nullptr){
        SDL_DestroyTexture((SDL_Texture*)textureID);
        this->textureID = nullptr;
    }
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

glm::vec3 RayTracingRenderer::fragmentFunction(glm::vec2 coord){

    // (bx^2 + by^2)t^2 + 2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0 
    // a => ray origin
    // b => ray direction
    // r => sphere radius
    // t => hit point

    glm::vec3 rayDirection = glm::vec3( coord.x, coord.y, -1.f);
    glm::vec3 rayOrigin = glm::vec3( 0.f, 0.f, 2.f );
    float r = 0.5f;
    // rayDirection = glm::normalize(rayDirection);

    // float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - r*r;
    

    // discriminante
    float delta = b*b -4.f*a*c;

    if( delta >= 0 )
        return glm::vec3( 0.f, 1.f, 0.f );
    else
        return glm::vec3( 69.f, 69.f, 69.f );
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

            color = vec3ToARGB( fragmentFunction( screenCoord ));
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

void RayTracingRenderer::takeScreenshot(){
    this->saveFrame = true;
}

uint32_t RayTracingRenderer::vec3ToARGB(const glm::vec3 &color){
    uint32_t a = 255 & 0xFF;
    uint32_t r = static_cast<uint32_t>(color.r * 255) & 0xFF;
    uint32_t g = static_cast<uint32_t>(color.g * 255) & 0xFF;
    uint32_t b = static_cast<uint32_t>(color.b * 255) & 0xFF;

    return (a << 24) | (r << 16) | (g << 8) | b;
}
