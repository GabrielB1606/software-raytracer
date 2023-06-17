#include "RayTracingRenderer.h"

RayTracingRenderer::RayTracingRenderer(SDL_Renderer *renderer){
    this->renderer = renderer;
}

void RayTracingRenderer::putpixel(glm::vec2 position, glm::vec3 color)
{
    SDL_Rect pixelRect = { (int)position.x, (int)position.y, 1, 1 };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255.f);
    SDL_RenderFillRect(renderer, &pixelRect);
}
