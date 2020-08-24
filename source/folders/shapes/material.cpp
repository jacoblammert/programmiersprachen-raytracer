//
// Created by Jacob Lammert on 17.08.2020.
//

#include "material.hpp"

Material::Material (float roughness, float glossy, float, float transparency, float refractiveIndex, glm::vec3 const& color) :

    roughness {roughness},
    glossy {glossy},
    transparency {transparency},
    refractiveIndex {refractiveIndex},
    color {color}

{}
