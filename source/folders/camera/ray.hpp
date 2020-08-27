#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP


#include <glm-0.9.5.3/glm/geometric.hpp>

struct Ray {
    //TODO put in math folder direction
    glm::vec3 position_;
    glm::vec3 direction_;
    Ray(const glm::vec3& position,const glm::vec3& direction);
};


#endif //RAYTRACER_RAY_HPP
