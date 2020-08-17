//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "shape.h"

class sphere : public shape {

    //TODO check if name should be Sphere or sphere

    sphere(const glm::vec3& position, float radius);

    //sphere(glm::vec3 position, float radius, Material* material);

    bool getIntersectVec(ray ray, glm::vec3 &HitPoint,  glm::vec3 &HitNormal,float &distance)const override;

    glm::vec3 getNormal(glm::vec3 pos)const override;

    glm::vec3 getMin()const override;

    glm::vec3 getMax()const override;

    void translate(glm::vec3 position) override;

    glm::vec3 getMedian()const override;

    //Material* getMaterial() override;

    //void setMaterial(Material* material) override;

    void print()const override;

private:
    glm::vec3 pos;
    float radius;
};


#endif //RAYTRACER_SPHERE_H
