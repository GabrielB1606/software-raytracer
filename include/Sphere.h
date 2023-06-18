#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape{
private:
    float radius;
public:
    Sphere(glm::vec3 origin, float radius);
    virtual ~Sphere(){}

    Ray reflect(Ray r) override {return Ray();};
    float hit(Ray r) override;

    float getRadius();
    void setRadius(float r);
};

#endif