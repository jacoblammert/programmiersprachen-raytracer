#include "material.hpp"

/**
 *
 * @param roughness of the surface
 * @param glossy / reflectivity
 * @param transparency
 * @param refractive_index_
 * @param color r g b
 */
Material::Material(float roughness, float glossy, float transparency, float refractiveIndex, glm::vec3 const &color) :

        roughness_{roughness},
        glossy_{glossy},
        transparency_{transparency},
        refractive_index_{refractiveIndex},
        color_{color} {};

/**
 *
 * @param roughness of the surface
 * @param glossy / reflectivity
 * @param transparency
 * @param refractive_index_
 * @param abberationStrength how different the wavelengths are refracted
 * @param color r g b
 */
Material::Material(float roughness, float glossy, float transparency, float refractiveIndex, float abberationStrength,
                   const glm::vec3 &color) :
        roughness_{roughness},
        glossy_{glossy},
        transparency_{transparency},
        refractive_index_{refractiveIndex},
        aberration_strength_{abberationStrength},
        color_{color} {}
