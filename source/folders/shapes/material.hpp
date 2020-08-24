//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Material {
    float roughness;
    float glossy;
    float transparency;
    float refractiveIndex;
    glm::vec3 color;
    
    Material (float roughness, float glossy, float, float transparency, float refractiveIndex, glm::vec3 const& color);
};


#endif //RAYTRACER_MATERIAL_H
