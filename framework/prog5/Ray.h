
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include <glm-0.9.5.3/glm/vec3.hpp>

struct Ray
{
    glm::vec3 origin_ = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction_ = {0.0f, 0.0f, -1.0f};
};


#endif //RAYTRACER_RAY_H
