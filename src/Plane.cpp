#include "Plane.h"

Plane::Plane(glm::vec3 point, glm::vec3 normal){
    this->albedo = glm::vec3(1.f, 0.6f, 0.6f);
    this->point = point;
    this->normal = normal;
}

float Plane::hit(Ray r){

    float denom = glm::dot(normal, r.direction);
    if (abs(denom) > 0.0001f) { // avoid division by zero
        float t = glm::dot(point - r.origin, normal) / denom;
        if (t >= 0) 
            return t;
    }
    return -1.0f;
}

glm::vec3 Plane::normalAt(glm::vec3 p){
    return this->normal;
}