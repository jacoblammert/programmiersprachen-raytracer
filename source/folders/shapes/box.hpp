//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_BOX_H
#define RAYTRACER_BOX_H

#include <cmath>
#include <iostream>
#include <vector>

#include "shape.hpp"

class Box : public Shape {
public:
    Box();

    Box(const glm::vec3 &minXminYminZ, const glm::vec3 &maxXmaxYmaxZ);

    Box(glm::vec3 const& pos, float xScale, float yScale, float zScale);

    void translate(glm::vec3 const& position) override;


    //TODO check, if so many functions are even necessary for skybox, if we want to implement one later or the Kd-Tree / hierarchy
    bool get_intersect_vec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    //void getIntersectVec(glm::vec3 &direction,  glm::vec3 &HitNormal) const; /// für Skybox

    bool get_intersect(const Ray &ray) const;

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

/*/
    Material* getMaterial() override;

    void setMaterial(Material* material) override;
/**/
    void print() const override;

private:
    int sign (glm::vec3 const& vec3, int position) const;

    std::vector<glm::vec3> bounds_;

    glm::vec3 position_;
};


#endif //RAYTRACER_BOX_H
