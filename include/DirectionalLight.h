#ifndef LIGHT_H
#define LIGHT_H

#include <algorithm>
#include <cmath>

#include "glm/glm.hpp"
#include "Shape.h"

enum LIGHT_MODEL {
    PHONG = 0,
    OREN_NAYAR,
    COOK_TORRANCE,
    LIGHT_MODEL_COUNT
};

struct DirectionalLight{


    LIGHT_MODEL model = PHONG;
    glm::vec3 direction;
    float intensity = 0.5f;

    glm::vec3 color = glm::vec3(1.f);
    float ambient = 0.15f;
    float specularStrength = 1.f;

    DirectionalLight(float x, float y, float z){
        this->direction = glm::normalize( glm::vec3(x, y, z));
    }

    glm::vec3 phong( Shape* shape, glm::vec3 normal, glm::vec3 viewDirection );
    glm::vec3 oren_nayar( Shape* shape, glm::vec3 normal, glm::vec3 viewDirection );

};


#endif