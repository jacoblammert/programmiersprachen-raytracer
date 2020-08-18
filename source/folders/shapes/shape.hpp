//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <glm-0.9.5.3/glm/geometric.hpp>
#include "../camera/ray.hpp"

class Shape {
public:

    virtual bool getIntersectVec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &Hitnormal,
                                 float &distance) const = 0; // returns true, if the Ray and the object intersect

    virtual glm::vec3 getNormal(glm::vec3 const& pos) const = 0;

    virtual glm::vec3 getMin() const = 0;

    virtual glm::vec3 getMax() const = 0;

    virtual glm::vec3 getMedian() const = 0;

    //virtual Material* getMaterial() = 0; // needs override for composite object

    //virtual void setMaterial(Material* material) = 0; // needs override for composite object

    virtual void print() const = 0;

    virtual void translate(
            glm::vec3 const& position) = 0;
    //TODO add rotateX(float angle (degrees)), rotateY(float angle (degrees)), rotateZ(float angle (degrees)) + scale(float size)

protected:
    //Material* material;
};


#endif //RAYTRACER_SHAPE_H
