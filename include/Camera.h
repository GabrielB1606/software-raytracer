#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

#include "Ray.h"

struct Camera{
    glm::vec3 position;
    float z_direction;

    Ray getRay(glm::vec2 screenCoord){
        Ray r;
        r.origin = this->position;
        r.direction = glm::vec3(screenCoord, this->z_direction);
        return r;
    }
    
};

#endif