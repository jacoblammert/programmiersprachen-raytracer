#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP


#include <glm-0.9.5.3/glm/geometric.hpp>

struct Ray {
    glm::vec3 position;
    glm::vec3 direction;
    Ray(const glm::vec3& position,const glm::vec3& direction);
};


#endif //RAYTRACER_RAY_HPP
