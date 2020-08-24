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

    virtual bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal,
                                 float &distance) const = 0; // returns true, if the Ray and the object intersect

    virtual glm::vec3 get_normal(glm::vec3 const& pos) const = 0;

    virtual glm::vec3 get_min() const = 0;

    virtual glm::vec3 get_max() const = 0;

    virtual glm::vec3 get_median() const = 0;

    //virtual Material* get_material() = 0; // needs override for composite object

    //virtual void set_material(Material* material) = 0; // needs override for composite object

    virtual void print() const = 0;

    virtual void translate(glm::vec3 const& position) = 0;
    //TODO add rotate_x (float angle (degrees)), rotateY(float angle (degrees)), rotateZ(float angle (degrees)) + scale(float size)

protected:
    //Material* material_;
};


#endif //RAYTRACER_SHAPE_H
