#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <string>

struct Material {
    float roughness = 0.0f;
    float glossy = 0.0f;
    float opacity = 1.0f;
    float refractive_index = 1.0f;
    // chromatic aberration on lenses (red, green, blue are refracted differently) (transparency > 0)
    float aberration_strength = 0;
    glm::vec3 color_ambient = glm::vec3 {0, 0, 0};
    glm::vec3 color_diffuse = glm::vec3 {0, 0, 0};
    glm::vec3 color_specular = glm::vec3 {0, 0, 0};
    float reflective_exponent = 1;
    std::string name;

    Material(std::string const& name, glm::vec3 const& color_ambient, glm::vec3 const& color_diffuse, glm::vec3 const& color_specular, float reflective_exponent);
    Material (glm::vec3 const& color_ambient, glm::vec3 const& color_diffuse, glm::vec3 const& color_specular, float reflective_exponent);
    Material (glm::vec3 const& color_ambient, glm::vec3 const& color_diffuse, glm::vec3 const& color_specular, float reflective_exponent, float opacity, float refractive_index);
    Material (float roughness, float glossy, float opacity, float refractiveIndex, glm::vec3 const& color);
    Material (float roughness, float glossy, float opacity, float refractiveIndex, float abberationStrength, glm::vec3 const& color);
};


#endif //RAYTRACER_MATERIAL_HPP
