#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "SDL.h"

class RayTracingRenderer{
private:
    SDL_Renderer* renderer;
public:
    RayTracingRenderer(SDL_Renderer* renderer);
    virtual ~RayTracingRenderer(){}

    void putPixel(glm::vec2 position, glm::vec3 color);
    glm::vec3 fragmentFunction(glm::vec2 coord);

    void render(size_t width, size_t height);
};


#endif
