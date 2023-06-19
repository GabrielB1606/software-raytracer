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

glm::vec3 DirectionalLight::oren_nayar(Shape *shape, glm::vec3 normal, glm::vec3 viewDirection){

    glm::vec3 ambientComponent = ambient * color;

    viewDirection = -glm::normalize(viewDirection);
    glm::vec3 lightDirection = glm::normalize(direction);

    float ndotv = std::clamp(glm::dot(normal, viewDirection), 0.f, 1.f);
    float angleVN = std::acos(ndotv); // theta_r

    float ndotl = std::clamp(glm::dot(normal, lightDirection), 0.f, 1.f);
    float angleLN = std::acos(ndotl); // theta_i

    float cos_phi_diff =
        glm::clamp(
            glm::dot(
                glm::normalize( viewDirection - normal * ndotv ),
                glm::normalize(lightDirection-normal *ndotl)
            )
        , 0.f, 1.f );

    float alpha = std::max(angleVN, angleLN);
    float beta =  std::min(angleVN, angleLN);

    float roughness = shape->getRoughness();

    float A = 1.0 - 0.5 * (roughness / (roughness + 0.33));
    float B = 0.45 * (roughness / (roughness + 0.09));

    glm::vec3 diffuseComponent = color * ( glm::clamp(ndotl, 0.f, 1.f) * (A + B * std::sin(alpha) * std::tan(beta) ));

    glm::vec3 finalColor = (ambientComponent + diffuseComponent) * shape->getAlbedo();

    return finalColor;
}
