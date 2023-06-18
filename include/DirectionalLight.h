#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

struct DirectionalLight{

    enum LIGHT_MODEL {
        PHONG = 0,
        OREN_NAYAR,
        COOK_TORRANCE
    };

    LIGHT_MODEL model = PHONG;
    glm::vec3 direction;
    float intensity = 0.5f;

    DirectionalLight(float x, float y, float z){
        this->direction = glm::normalize( glm::vec3(x, y, z));
    }

};


#endif