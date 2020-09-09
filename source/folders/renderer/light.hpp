#ifndef RAYTRACER_LIGHT_HPP
#define RAYTRACER_LIGHT_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <string>

struct Light {

    Light (std::string const& name, glm::vec3 const& position, glm::vec3 const& color, glm::vec3 const& brightness, float hardness = 1.0);

    std::string name;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 brightness;
    float hardness = 1.0f;

};


#endif //RAYTRACER_LIGHT_HPP
