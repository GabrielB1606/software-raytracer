#include "DirectionalLight.h"

glm::vec3 DirectionalLight::phong(Shape* shape, glm::vec3 normal, glm::vec3 viewDirection){

    // glm::vec3 ambientComponent = ambient * color;

    glm::vec3 diffuseComponent = __max( 0.f, glm::dot(normal, direction) ) * color;

    viewDirection = glm::normalize(viewDirection);
    glm::vec3 reflectDir = glm::reflect(direction, normal);
    float spec = pow(__max(glm::dot(viewDirection, reflectDir), 0.f), shape->getShininess());

    glm::vec3 specularComponent = specularStrength * spec * color;

    return ( diffuseComponent + specularComponent) * shape->getAlbedo();
}

glm::vec3 DirectionalLight::oren_nayar(Shape *shape, glm::vec3 normal, glm::vec3 viewDirection){

    // glm::vec3 ambientComponent = ambient * color;

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

    glm::vec3 finalColor = (diffuseComponent) * shape->getAlbedo();

    return finalColor;
}

glm::vec3 DirectionalLight::cook_torrance(Shape *shape, glm::vec3 normal, glm::vec3 viewDirection){

    glm::vec3 lightDirection = glm::normalize(direction);
    viewDirection = -glm::normalize(viewDirection);

    float brdf = 0.f;
    glm::vec3 halfVector = glm::normalize(lightDirection + viewDirection);
    float NdotL = std::max(glm::dot(normal, lightDirection), 0.0f);
    float NdotH = std::max(glm::dot(normal, halfVector), 0.0f);
    float NdotV = std::max(glm::dot(normal, viewDirection), 0.0f);
    float VdotH = std::max(glm::dot(viewDirection, halfVector), 0.0f);

    // fresnel
    // float Kr = glm::pow((1.f - 2.f) / (1.f + 2.f), 2.f);
    float Kr = shape->getFresnel();
    float F = Kr + (1.0 - Kr) * pow((1.0 - NdotL), 5.0);

    // Distribución Normal Trowbridge-Reitz = m^2 / (1 - m^2) * cos^2(alpha) - 1
    float NH2 = glm::pow(NdotH, 2.f);
    float roughness2 = glm::pow(glm::clamp( shape->getRoughness(), 0.01f, 0.99f), 2.0f);
    float denom = NH2 * roughness2 + (1.0 - NH2);
    float D = roughness2 / (M_PI * pow(denom, 2.0));
    
    // geometry
    float g1 = (NdotL * 2.0) / (NdotL + std::sqrt(roughness2 + (1.0 - roughness2) * glm::pow(NdotL, 2.0)));
    float g2 = (NdotV * 2.0) / (NdotV + std::sqrt(roughness2 + (1.0 - roughness2) * glm::pow(NdotV, 2.0)));
    float G = g1 * g2;

    brdf = (F*G*D)/((M_PI * NdotV));
    glm::vec3 ct = this->specularStrength * ( brdf* this->color );
    
    glm::vec3 lambertian = (1.f - shape->getRoughness()) * (this->color * NdotL);

    glm::vec3 final = shape->getAlbedo() * (ct + lambertian);
    final = glm::pow(final, glm::vec3(1.f / 2.2f));

    return final;

}
