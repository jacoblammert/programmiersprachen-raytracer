#ifndef RAYTRACER_LIGHT_HPP
#define RAYTRACER_LIGHT_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Light {

    /**
     *
     * @param position as vec 3
     * @param color as vec 3 (red, gren, blue)
     * @param brightness as float
     * @param hardness as float => 1 = hard shadows, 0 = really soft shadows
     */
    Light(glm::vec3 const& position, glm::vec3 const& color, float brightness,float hardness = 1):
            position{position},color{color},brightness{brightness},hardness{hardness}
    {}

    glm::vec3 position;
    glm::vec3 color;
    float brightness;
    float hardness;

};


#endif //RAYTRACER_LIGHT_HPP
