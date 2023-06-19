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
    float specularExponent;
    float roughness = 0.f;
    float smoothness = 1.f;
    float shininess = 32.f;
    float eta = 1.f;
    float fresnel = 0.5f;

    bool refractive = false;

public:
    // Should return the T parameter for the closest hit
    virtual float hit(Ray r) = 0;

    // Should return the ray reflected on the object
    virtual Ray reflect(Ray r) = 0;   

    // Should retur the normal vector at certain point
    virtual glm::vec3 normalAt(glm::vec3 p) = 0;

    virtual glm::vec3 getPosition(){ return this->position; }
    glm::vec3 getAlbedo(){ return this->albedo; }
    float getScale(){ return this->scale; }
    float getShininess(){ return this->shininess; }
    float getRoughness(){ return this->roughness; }
    bool isRefractive(){ return this->refractive; }
    float getETA(){ return this->eta; }
    float getFresnel(){ return this->fresnel; }

    void setAlbedo(glm::vec3 albedo){ this->albedo = albedo; }
    virtual void setPosition(glm::vec3 position){ this->position = position; }
    void setScale(float s){ this->scale = s; }
    void setRoughness(float r){ this->roughness = r; }
    void setRefractive(bool r){ this->refractive = r; }
    void setETA(float eta){ this->eta = eta; }
    void setFresnel(float f){ this->fresnel = f; }

};

#endif