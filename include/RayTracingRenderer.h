#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include <iostream>
#include <vector>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SDL.h"
#include "imgui.h"

#include "Camera.h"
#include "Shape.h"
#include "Sphere.h"

class RayTracingRenderer{
private:
    SDL_Surface *surface;
    size_t width, height;
    ImTextureID textureID;
    bool saveFrame = false;

    std::vector<Shape*> scene;
    Camera cam;

    glm::vec3 clearColor = glm::vec3(69.f);

public:
    RayTracingRenderer(size_t width, size_t height);
    virtual ~RayTracingRenderer();

    void putPixel(glm::vec2 position, glm::vec3 color);
    glm::vec3 fragmentFunction(glm::vec2 coord);

    ImTextureID render(SDL_Renderer* renderer);

    void takeScreenshot();

    void addShape(Shape* shape);
    Shape* getShape(size_t index);
    size_t getSceneSize();

    Camera* getCamRef();

    glm::vec3 getClearColor();
    void setClearColor(glm::vec3 color);

    static uint32_t vec3ToARGB(const glm::vec3& color);

};


#endif
