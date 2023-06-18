#include "Cube.h"

Cube::Cube(glm::vec3 origin, float size){
    this->albedo = glm::vec3(0.f, 1.f, 1.f);
    this->position = origin;
    this->scale = size;
}

float Cube::hit(Ray r){

    float half = this->scale/2.f;
    bMin = glm::vec3( this->position.x - half, this->position.y - half, this->position.z - half );
    bMax = glm::vec3( this->position.x + half, this->position.y + half, this->position.z + half );

    float tx1, tx2, ty1, ty2, tz1, tz2, tNear, tFar;
    tx1 = (bMin.x - r.origin.x) / r.direction.x;
    tx2 = (bMax.x - r.origin.x) / r.direction.x;
    ty1 = (bMin.y - r.origin.y) / r.direction.y;
    ty2 = (bMax.y - r.origin.y) / r.direction.y;
    tz1 = (bMin.z - r.origin.z) / r.direction.z;
    tz2 = (bMax.z - r.origin.z) / r.direction.z;

    tNear = std::max(std::min(tx1, tx2), std::max(std::min(ty1, ty2), std::min(tz1, tz2)));
    tFar = std::min(std::max(tx1, tx2), std::min(std::max(ty1, ty2), std::max(tz1, tz2)));

    if (tNear > tFar || tFar < 0) {
        return -1;
    }

    return tNear;

}

glm::vec3 Cube::normalAt(glm::vec3 p){
    
    glm::vec3 n = glm::vec3(p.x - position.x, p.y - position.y, p.z - position.z);
    glm::vec3 nAbs = glm::abs(n);

    float maxVal = __max(__max(nAbs.x, nAbs.y), nAbs.z);

    // Check which face of the cube the point lies on based on the largest absolute difference
    if (maxVal == nAbs.x) {
        return { n.x>0?1.f:-1.f, 0.f, 0.f };
    } else if (maxVal == nAbs.y) {
        return { 0.f, n.y>0?1.f:-1.f, 0.f };
    } 
    
    return { 0.f, 0.f, n.z>0?1.f:-1.f };

}
