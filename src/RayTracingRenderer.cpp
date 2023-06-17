#include "RayTracingRenderer.h"

RayTracingRenderer::RayTracingRenderer(SDL_Renderer *renderer){
    this->renderer = renderer;
}

void RayTracingRenderer::putPixel(glm::vec2 position, glm::vec3 color)
{

    // SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255.f);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
    // SDL_Rect pixelRect = { (int)position.x, (int)position.y, 1, 1 };
    // SDL_RenderFillRect(renderer, &pixelRect);
}

glm::vec3 RayTracingRenderer::fragmentFunction(glm::vec2 coord){
    return glm::vec3(69.f);
}

void RayTracingRenderer::render(size_t width, size_t height){

    // SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_SetRenderDrawColor(renderer,69, 69, 69, 255.f);
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            putPixel(glm::vec2(j, i), fragmentFunction(glm::vec2(j, i)));

}