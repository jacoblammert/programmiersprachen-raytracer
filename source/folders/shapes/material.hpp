#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Material {
    float roughness_ = 0.0f;
    float glossy_ = 0.0f;
    float opacity_ = 0.0f; /// transparency
    float refractive_index_ = 1.0f;
    float aberration_strength_ = 0; // chromatic aberration on lenses (red, green, blue are refracted differently) (transparency > 0)

    glm::vec3 color_ambient_ = glm::vec3 {0,0,0};
    glm::vec3 color_diffuse_ = glm::vec3 {0,0,0};
    glm::vec3 color_specular_ = glm::vec3 {0,0,0};
    float reflective_exponent_ = 1;

    Material (glm::vec3 const& color_ambient,glm::vec3 const& color_diffuse,glm::vec3 const& color_specular,float reflective_exponent);
    Material (glm::vec3 const& color_ambient,glm::vec3 const& color_diffuse,glm::vec3 const& color_specular,float reflective_exponent,float opacity, float refractive_index);

    Material (float roughness, float glossy, float opacity, float refractiveIndex, glm::vec3 const& color);
    Material (float roughness, float glossy, float opacity, float refractiveIndex, float abberationStrength, glm::vec3 const& color);
};


#endif //RAYTRACER_MATERIAL_HPP
