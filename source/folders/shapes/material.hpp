#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Material {
    float roughness_;
    float glossy_;
    float transparency_;
    float refractive_index_;
    float aberration_strength_ = 0; // chromatic aberration on lenses (red, green, blue are refracted differently) (transparency > 0)
    glm::vec3 color_;

    Material (float roughness, float glossy, float transparency, float refractiveIndex, glm::vec3 const& color);

    Material (float roughness, float glossy, float transparency, float refractiveIndex, float abberationStrength, glm::vec3 const& color);
};


#endif //RAYTRACER_MATERIAL_HPP
