

#ifndef RAYTRACER_BOX_H
#define RAYTRACER_BOX_H


#include <glm-0.9.5.3/glm/vec3.hpp>
#include "Shape.h"

class Box: Shape {
public:
    Box(glm::vec3 min,glm::vec3 max);
    float const area() override;
    float const volume() override;

private:
    glm::vec3 max_;
    glm::vec3 min_;
};


#endif //RAYTRACER_BOX_H
