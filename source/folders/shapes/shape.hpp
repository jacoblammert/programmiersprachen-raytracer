#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <glm-0.9.5.3/glm/geometric.hpp>
#include <memory>
#include <string>
#include <glm-0.9.5.3/glm/detail/type_mat3x3.hpp>
#include "../camera/ray.hpp"
#include "material.hpp"

enum ShapeType {
    BOX, CONE, CYLINDER, PLANE, SPHERE, TRIANGLE, COMPOSITE
};


class Shape {
public:

    virtual bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal,
                                 float &distance) const = 0; // returns true, if the Ray and the object intersect

    virtual glm::vec3 get_normal(glm::vec3 const& pos) const = 0;

    virtual glm::vec3 get_min() const = 0;

    virtual glm::vec3 get_max() const = 0;

    virtual glm::vec3 get_median() const = 0;

    virtual std::shared_ptr<Material> get_material() = 0; // needs override for composite object

    virtual void set_material (std::shared_ptr<Material> const& material) = 0; // needs override for composite object

    virtual void print(std::fstream & file) const = 0;

    virtual void translate(glm::vec3 const& position) = 0;

    glm::vec3 get_translated_vec3(glm::vec3 const& vec3, glm::vec3 const& translation) const;

    glm::vec3 get_scaled_vec3(glm::vec3 const& vec3, float scale_x,float scale_y,float scale_z) const;

    glm::vec3 get_rotated_vec3(glm::vec3 const& vec3,glm::vec3 axis, float angle) const;

    void set_angle(float angle);

    void set_rotation_axis(const glm::vec3& axis);

    std::string get_name() const;

    virtual std::string get_information() const = 0;
    
    ShapeType get_shape_type() const;

protected:
    glm::vec3 axis_ = glm::vec3 {};
    glm::mat3x3 rotation_matrix_ = glm::mat3x3{1,0,0,0,1,0,0,0,1};
    glm::mat3x3 rotation_matrix_inverse = glm::mat3x3{1,0,0,0,1,0,0,0,1};
    float angle_ = 0.0f;
    std::shared_ptr<Material> material_;
    ShapeType shape_type_;
    std::string name_;
    glm::vec3 position_ = glm::vec3 {};
};


#endif //RAYTRACER_SHAPE_H
