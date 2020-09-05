#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include <cmath>
#include <iostream>
#include "shape.hpp"

class Sphere : public Shape {

public:
    Sphere (std::string const& name, const glm::vec3& position, float radius);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point,  glm::vec3 &hit_normal, float &distance) const override;

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

    std::shared_ptr<Material> get_material() override;

    void set_material (std::shared_ptr<Material> const& material) override;

    void print(std::fstream & file) const override;
    
    void translate(glm::vec3 const& position) override;

    std::string get_information()const override;

private:
    float radius_;
};


#endif //RAYTRACER_SPHERE_HPP
