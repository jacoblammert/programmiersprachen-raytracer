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

        roughness{roughness},
        glossy{glossy},
        opacity{transparency},
        refractive_index{refractiveIndex},
        color_ambient{color_ambient} {};

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
        roughness{roughness},
        glossy{glossy},
        opacity{opacity},
        refractive_index{refractiveIndex},
        aberration_strength{abberationStrength},
        color_ambient{color} {}

/**
 * Material definition from sdf format
 * @param name name of the material
 * @param color_ambient
 * @param color_diffuse
 * @param color_reflective
 * @param reflective_exponent
 */
Material::Material (std::string const& name, const glm::vec3 &color_ambient, const glm::vec3 &color_diffuse, const glm::vec3 &color_specular,
                    float reflective_exponent) :
        name{name},
        color_ambient{color_ambient},
        color_diffuse{color_diffuse},
        color_specular{color_specular},
        reflective_exponent{reflective_exponent} {
    
}

/**
 * Material definition from sdf format
 * @param color_ambient
 * @param color_diffuse
 * @param color_reflective
 * @param reflective_exponent
 */
Material::Material(const glm::vec3 &color_ambient, const glm::vec3 &color_diffuse, const glm::vec3 &color_specular,
                   float reflective_exponent) :
        color_ambient{color_ambient}, color_diffuse{color_diffuse}, color_specular{color_specular},
        reflective_exponent{reflective_exponent} {

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
        color_ambient{color_ambient}, color_diffuse{color_diffuse}, color_specular{color_specular},
        reflective_exponent{reflective_exponent}, opacity{opacity}, refractive_index{refractive_index} {

}
