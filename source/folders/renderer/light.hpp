//
// Created by Jacob Lammert on 21.08.2020.
//

#ifndef RAYTRACER_LIGHT_HPP
#define RAYTRACER_LIGHT_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Light {

    //Light(glm::vec3 position,glm::vec3 color, float brightness);
    Light(glm::vec3 position, glm::vec3 color, float brightness):
            position{position},color{color},brightness{brightness}{

    }
    glm::vec3 position;
    glm::vec3 color;
    float brightness;

};


#endif //RAYTRACER_LIGHT_HPP
