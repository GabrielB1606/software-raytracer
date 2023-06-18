#ifndef SHAPE_H
#define SHAPE_H

#include "glm/glm.hpp"
#include "Ray.h"

class Shape{
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
    glm::vec3 albedo;

public:
    // Should return the T parameter for the closest hit
    virtual float hit(Ray r) = 0;

    // Should return the ray reflected on the object
    virtual Ray reflect(Ray r) = 0;   

    glm::vec3 getPosition(){ return this->position; }
    glm::vec3 getAlbedo(){ return this->albedo; }
};

#endif