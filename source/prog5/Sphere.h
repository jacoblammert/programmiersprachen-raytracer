//
// Created by Jacob on 12.07.2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Shape.h"

class Sphere: Shape {
public:
    Sphere(glm::vec3 center, float radius);
    float const area() override;
    float const volume() override;

private:
    glm::vec3 center_;
    float radius_;
};


#endif //RAYTRACER_SPHERE_H
