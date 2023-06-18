#include "Sphere.h"

Sphere::Sphere(glm::vec3 origin, float radius){
    this->position = origin;
    this->radius = radius;
    this->albedo = glm::vec3(0.f, 1.f, 0.f);
}

float Sphere::hit(Ray r){

    // (bx^2 + by^2)t^2 + 2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0 
    // a => ray origin
    // b => ray direction
    // t => hit point

    float a = glm::dot(r.direction, r.direction);
    float b = 2.f * glm::dot(r.origin, r.direction);
    float c = glm::dot(r.origin, r.origin) - radius*radius;

    // discriminante
    float delta = b*b -4.f*a*c;

    if(delta >= 0.f){

        float t0 = (-b-sqrtf(delta))/(2.f*a);
        float t1 = (-b+sqrtf(delta))/(2.f*a); 

        if( t0 >= 0.f && t1 >= 0.f )
            return __min( t0, t1 );
        else if( t0>=0.f )
            return t0;
        else if( t1>=0.f )
            return t1;

    }
    
    return -1.f;

}

float Sphere::getRadius(){
    return radius;
}

void Sphere::setRadius(float r){
    this->radius = r;
}
