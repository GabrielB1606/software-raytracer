#include "Plane.h"

Plane::Plane(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pd): a(pa), b(pb), c(pc), d(pd){

    this->albedo = glm::vec3(1.f,1.f,0.f);
    this->normal = glm::normalize( glm::cross((b - a), (d-a)) );

}

float Plane::hit(Ray r){
    
    glm::vec3 vdif = a - r.origin;
	float vdotn = glm::dot(r.direction, normal);

	if(fabs(vdotn) < 1.e-4)
        return -1.f;

    float t = glm::dot(vdif, normal)/vdotn;
	if(fabs(t) < 0.0001)
        return -1.f;

	glm::vec3 q = r.at(t);

	if(isInside(q))
        return t;
    else
        return -1.f;
}

glm::vec3 Plane::normalAt(glm::vec3 p){
    return this->normal;
}

bool Plane::isInside(glm::vec3 pt){

    glm::vec3 uA = b - a;
    glm::vec3 uB = c - b;
    glm::vec3 uC = d - c;
    glm::vec3 uD = a - d;

    glm::vec3 vA = pt - a;
    glm::vec3 vB = pt - b;
    glm::vec3 vC = pt - c;
    glm::vec3 vD = pt - d;

    float A = glm::dot(glm::cross(uA, vA), normal);
    float B = glm::dot(glm::cross(uB, vB), normal);
    float C = glm::dot(glm::cross(uC, vC), normal);
    float D = glm::dot(glm::cross(uD, vD), normal);

    return ((A > 0) && (B > 0) && (C > 0) && (D > 0));
}
