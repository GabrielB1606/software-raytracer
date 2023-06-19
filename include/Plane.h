#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane: public Shape{
private:
    glm::vec3 point; // a point on the plane
    glm::vec3 normal; // the normal to the plane
public:
    Plane(glm::vec3 point, glm::vec3 normal);

    virtual ~Plane(){};

    Ray reflect(Ray r) override {return Ray();};
    float hit(Ray r) override;
    glm::vec3 normalAt(glm::vec3 p) override;

    glm::vec3 getPosition() override;
    void setPosition(glm::vec3 p) override;

    glm::vec3* getNormalRef();

};

#endif