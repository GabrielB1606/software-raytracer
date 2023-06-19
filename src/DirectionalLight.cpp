#include "DirectionalLight.h"

glm::vec3 DirectionalLight::phong(Shape* shape, glm::vec3 normal, glm::vec3 viewDirection){

    glm::vec3 ambientComponent = ambient * color;

    glm::vec3 diffuseComponent = __max( 0.f, glm::dot(normal, direction) ) * color;

    viewDirection = glm::normalize(viewDirection);
    glm::vec3 reflectDir = glm::reflect(direction, normal);
    float spec = pow(__max(glm::dot(viewDirection, reflectDir), 0.f), shape->getShininess());

    glm::vec3 specularComponent = specularStrength * spec * color;

    return (ambientComponent + diffuseComponent + specularComponent) * shape->getAlbedo();
}
