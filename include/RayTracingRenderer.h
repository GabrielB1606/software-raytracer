#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include <iostream>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "SDL.h"
#include "imgui.h"

class RayTracingRenderer{
private:
    SDL_Surface *surface;
    size_t width, height;
    ImTextureID textureID;
    bool saveFrame = false;
public:
    RayTracingRenderer(size_t width, size_t height);
    virtual ~RayTracingRenderer();

    void putPixel(glm::vec2 position, glm::vec3 color);
    glm::vec3 fragmentFunction(glm::vec2 coord);

    ImTextureID render(SDL_Renderer* renderer);

    void takeScreenshot();

    static uint32_t vec3ToARGB(const glm::vec3& color);

};


#endif
