#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane: public Shape{
private:
    glm::vec3 a, b, c, d;
    glm::vec3 normal;
public:
    Plane(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pd);

    virtual ~Plane(){};

    Ray reflect(Ray r) override {return Ray();};
    float hit(Ray r) override;
    glm::vec3 normalAt(glm::vec3 p) override;

    bool isInside(glm::vec3 pt);

};

#endif