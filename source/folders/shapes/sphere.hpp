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

    Sphere(const glm::vec3& position, float radius);

    //sphere(glm::vec3 position, float radius, Material* material);

    bool getIntersectVec(Ray const& ray, glm::vec3 &HitPoint,  glm::vec3 &HitNormal,float &distance)const override;

    glm::vec3 getNormal(glm::vec3 const& pos) const override;

    glm::vec3 getMin() const override;

    glm::vec3 getMax() const override;

    void translate(glm::vec3 const& position) override;

    glm::vec3 getMedian() const override;

    //Material* getMaterial() override;

    //void setMaterial(Material* material) override;

    void print() const override;

private:
    glm::vec3 pos;
    float radius;
};


#endif //RAYTRACER_SPHERE_H
