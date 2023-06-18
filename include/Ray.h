#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"

struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 at(float t){
        return origin + t*direction;
    }
};

#endif