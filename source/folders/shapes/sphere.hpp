//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include <cmath>
#include <iostream>

#include "shape.hpp"

class Sphere : public Shape {

    //TODO check if name should be Sphere or sphere -> groß
public:
    Sphere(const glm::vec3& position, float radius);

    //sphere(glm::vec3 position, float radius, Material* material);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point,  glm::vec3 &hit_normal, float &distance) const override;

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    void translate(glm::vec3 const& position) override;

    glm::vec3 get_median() const override;

    //Material* getMaterial() override;

    //void setMaterial(Material* material) override;

    void print() const override;

private:
    glm::vec3 pos_;
    float radius_;
};


#endif //RAYTRACER_SPHERE_H
