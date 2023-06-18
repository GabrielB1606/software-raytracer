#ifndef CUBE_H
#define CUBE_H

#include <iostream>

#include "Shape.h"

class Cube : public Shape{
private:
    glm::vec3 bMin, bMax;
public:
    Cube(glm::vec3 origin, float size);
    virtual ~Cube(){};

    Ray reflect(Ray r) override {return Ray();};
    float hit(Ray r) override;
    glm::vec3 normalAt(glm::vec3 p) override;
};


#endif