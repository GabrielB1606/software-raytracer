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
    return glm::vec3(69.f);
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

    // Access pixel data and update framebuffer
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            // Write pixel data to framebuffer
            Uint32 color = vec3ToARGB(glm::vec3((float)x/width, (float)y/height, 0.f)) /* calculate or retrieve pixel color */;
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

    // SDL_Surface* surface = SDL_GetWindowSurface(window);
    // SDL_SetRenderDrawColor(renderer,69, 69, 69, 255.f);
    // for (size_t i = 0; i < height; i++)
    //     for (size_t j = 0; j < width; j++)
    //         putPixel(glm::vec2(j, i), fragmentFunction(glm::vec2(j, i)));

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
