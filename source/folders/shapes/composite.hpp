
//
// Created by Jacob Lammert on 20.08.2020.
//

#ifndef RAYTRACER_COMPOSITE_HPP
#define RAYTRACER_COMPOSITE_HPP

#include <utility>
#include <vector>
#include "shape.hpp"
#include "box.hpp"
#include "plane.hpp"


/**
 * This object is like a Kd-Tree and saves all the Shape pointers in a Vector.
 * The intersectionfunctions are different because the box must be intersected before any of the included Shapes are tested
 *
 *
 */
class Composite : public Shape {
public:

    Composite();

    Composite(int depth);

    Composite(std::vector<std::shared_ptr<Shape>> const& shapes);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const override;

    void get_intersected_shape(const Ray &ray,std::shared_ptr<Shape> & shape, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance);

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

    void translate(glm::vec3 const &position) override;

    std::shared_ptr<Material> get_material() override; //muss drin sein aber nicht sinnvoll?

    void set_material (std::shared_ptr<Material> const& material) override;
    
    void print() const override;

    void build();

    void add_shape(const std::shared_ptr<Shape>& shape);

    void add_shapes(std::vector<std::shared_ptr<Shape>> const& shapes);

private:

    void set_min_max_mid();
    
    void split();

    void get_min(glm::vec3 const& shape_min);

    void get_max(glm::vec3 const& shape_max);

    glm::vec3 min_x_y_z_;
    glm::vec3 max_x_y_z_;
    glm::vec3 median_;
    int depth_;
    Box box_;
    std::vector<Composite> boxes_; // TODO change from vector to two seperate objects
    std::vector<std::shared_ptr<Shape>> shapes_;
};


#endif //RAYTRACER_COMPOSITE_HPP
