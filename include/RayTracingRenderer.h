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
#include "Cube.h"
#include "DirectionalLight.h"

class RayTracingRenderer{
private:
    SDL_Surface *surface;
    size_t width, height;
    ImTextureID textureID;
    bool saveFrame = false;

    float aspectRatio;

    std::vector<Shape*> scene;
    std::vector<DirectionalLight*> lights;
    Camera cam;

    glm::vec3 clearColor = glm::vec3(0.f);
    float ambientIntensity = 0.05f;


    size_t bounces = 2;

public:
    bool activateShadow = false;
    bool realtime = true;

    struct TracingInfo{
        glm::vec3 hitPosition;
        Shape* hittedShape;
        float hitDistance;
    };


    RayTracingRenderer(size_t width, size_t height);
    virtual ~RayTracingRenderer();

    void changeRenderResolution(size_t width, size_t height);

    void putPixel(glm::vec2 position, glm::vec3 color);
    glm::vec4 fragmentFunction(glm::vec2 coord);

    TracingInfo traceRay(Ray r);

    ImTextureID render(SDL_Renderer* renderer);
    ImTextureID getLastFrame();

    void takeScreenshot();

    void addShape(Shape* shape);
    void removeShape(size_t index);
    Shape* getShape(size_t index);
    size_t getSceneSize();

    void addLight(DirectionalLight* light);
    void removeLight(size_t index);
    DirectionalLight* getLight(size_t index);
    size_t getLightsSize();

    Camera* getCamRef();

    glm::vec3 getClearColor();
    void setClearColor(glm::vec3 color);

    size_t* getBouncesRef(){ return &this->bounces; }

    static uint32_t vec3ToARGB(const glm::vec3& color);
    static uint32_t vec4ToARGB(const glm::vec4& color);

};


#endif
