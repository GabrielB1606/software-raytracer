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

    // brush
    void putpixel(glm::vec2 position, glm::vec3 color);
};


#endif
