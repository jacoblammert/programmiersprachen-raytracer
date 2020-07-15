
#ifndef RAYTRACER_HITPOINT_H
#define RAYTRACER_HITPOINT_H


#include <string>
#include <glm-0.9.5.3/glm/vec3.hpp>
#include <color.hpp>

struct HitPoint {
    bool hit_;
    float dist_;
    std::string name_;
    Color color_;
    glm::vec3 hitPosition_;
    glm::vec3 hitDirection_;
};


#endif //RAYTRACER_HITPOINT_H
