//
// Created by Jacob on 12.07.2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Shape.h"
#include "HitPoint.h"
#include "Ray.h"

class Sphere: Shape {
public:
    Sphere(glm::vec3 center, float radius,std::string name, Color color);
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os)const override;

    HitPoint intersect(Ray ray) const;

private:
    glm::vec3 center_;
    float radius_;
};


#endif //RAYTRACER_SPHERE_H
