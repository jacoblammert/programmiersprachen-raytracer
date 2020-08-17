//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_PLANE_H
#define RAYTRACER_PLANE_H

#include "shape.h"

class plane : public shape {
public:
    plane(glm::vec3 position, glm::vec3 normal);

    //plane(glm::vec3 position, glm::vec3 normal, Material* material);

    bool getIntersectVec(ray ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    glm::vec3 getNormal(glm::vec3 pos) const override;

    glm::vec3 getMin() const override;

    glm::vec3 getMax() const override;

    glm::vec3 getMedian() const override;


    void translate(glm::vec3 position) override;

/*/
    Material* getMaterial() override;
    void setMaterial(Material* material) override;
/**/
    void print() const override;

private:
    glm::vec3 pos;
    glm::vec3 normal;
};


#endif //RAYTRACER_PLANE_H
