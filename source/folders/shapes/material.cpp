#include "material.hpp"

/**
 *
 * @param roughness of the surface
 * @param glossy / reflectivity
 * @param transparency
 * @param refractive_index_
 * @param color r g b
 */
Material::Material(float roughness, float glossy, float transparency, float refractiveIndex,
                   glm::vec3 const &color_ambient) :

        roughness_{roughness},
        glossy_{glossy},
        opacity_{transparency},
        refractive_index_{refractiveIndex},
        color_ambient_{color_ambient} {};

/**
 *
 * @param roughness of the surface
 * @param glossy / reflectivity
 * @param transparency
 * @param refractive_index_
 * @param abberationStrength how different the wavelengths are refracted
 * @param color r g b
 */
Material::Material(float roughness, float glossy, float opacity, float refractiveIndex, float abberationStrength,
                   const glm::vec3 &color) :
        roughness_{roughness},
        glossy_{glossy},
        opacity_{opacity},
        refractive_index_{refractiveIndex},
        aberration_strength_{abberationStrength},
        color_ambient_{color} {}

/**
 * Material definition from sdf format
 * @param color_ambient
 * @param color_diffuse
 * @param color_reflective
 * @param reflective_exponent
 */
Material::Material(const glm::vec3 &color_ambient, const glm::vec3 &color_diffuse, const glm::vec3 &color_specular,
                   float reflective_exponent) :
        color_ambient_{color_ambient}, color_diffuse_{color_diffuse}, color_specular_{color_specular},
        reflective_exponent_{reflective_exponent} {

}

/**
 * Material definition from sdf format with transparency and a refractive index
 * @param color_ambient
 * @param color_diffuse
 * @param color_reflective
 * @param reflective_exponent
 * @param transparency
 * @param refractive_index
 */
Material::Material(const glm::vec3 &color_ambient, const glm::vec3 &color_diffuse, const glm::vec3 &color_specular,
                   float reflective_exponent, float opacity, float refractive_index) :
        color_ambient_{color_ambient}, color_diffuse_{color_diffuse}, color_specular_{color_specular},
        reflective_exponent_{reflective_exponent}, opacity_{opacity}, refractive_index_{refractive_index} {

}
