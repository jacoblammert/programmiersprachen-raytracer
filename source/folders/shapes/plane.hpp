//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_PLANE_H
#define RAYTRACER_PLANE_H

#include <iostream>
#include "shape.hpp"

class Plane : public Shape {
public:
    Plane(glm::vec3 const& position, glm::vec3 const& normal);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;


    void translate(glm::vec3 const& position) override;

/*
    Material* getMaterial() override;
    void setMaterial(Material* material) override;
*/
    void print() const override;

private:
    glm::vec3 pos_;
    glm::vec3 normal_;
};


#endif //RAYTRACER_PLANE_H
