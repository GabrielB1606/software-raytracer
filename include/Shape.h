#ifndef SHAPE_H
#define SHAPE_H

#include "glm/glm.hpp"
#include "Ray.h"

class Shape{
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    // material
    glm::vec3 albedo;
    float specular_component;
    float roughness;
    float metalic;

public:
    // Should return the T parameter for the closest hit
    virtual float hit(Ray r) = 0;

    // Should return the ray reflected on the object
    virtual Ray reflect(Ray r) = 0;   

    // Should retur the normal vector at certain point
    virtual glm::vec3 normalAt(glm::vec3 p) = 0;

    glm::vec3 getPosition(){ return this->position; }
    glm::vec3 getAlbedo(){ return this->albedo; }
    float getScale(){ return this->scale; }

    void setAlbedo(glm::vec3 albedo){ this->albedo = albedo; }
    void setPosition(glm::vec3 position){ this->position = position; }
    void setScale(float s){ this->scale = s; }

};

#endif