#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <string>

struct Material {
    float roughness_ = 0.0f;
    float glossy_ = 0.0f;
    float opacity_ = 1.0f;
    float refractive_index_ = 1.0f;
    float aberration_strength_ = 0; // chromatic aberration on lenses (red, green, blue are refracted differently) (transparency > 0)
    glm::vec3 color_ambient_ = glm::vec3 {0,0,0};
    glm::vec3 color_diffuse_ = glm::vec3 {0,0,0};
    glm::vec3 color_specular_ = glm::vec3 {0,0,0};
    float reflective_exponent_ = 1;
    std::string name_;

    Material(std::string const& name, glm::vec3 const& color_ambient, glm::vec3 const& color_diffuse, glm::vec3 const& color_specular, float reflective_exponent);
    Material (glm::vec3 const& color_ambient,glm::vec3 const& color_diffuse,glm::vec3 const& color_specular,float reflective_exponent);
    Material (glm::vec3 const& color_ambient,glm::vec3 const& color_diffuse,glm::vec3 const& color_specular,float reflective_exponent,float opacity, float refractive_index);

    Material (float roughness, float glossy, float opacity, float refractiveIndex, glm::vec3 const& color);
    Material (float roughness, float glossy, float opacity, float refractiveIndex, float abberationStrength, glm::vec3 const& color);
};


#endif //RAYTRACER_MATERIAL_HPP
