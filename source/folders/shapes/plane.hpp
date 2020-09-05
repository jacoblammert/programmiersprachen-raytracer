#ifndef RAYTRACER_PLANE_HPP
#define RAYTRACER_PLANE_HPP

#include <iostream>
#include "shape.hpp"

class Plane : public Shape {
public:
    Plane (std::string const& name, glm::vec3 const& position, glm::vec3 const& normal);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const override;

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
    glm::vec3 normal_;
};


#endif //RAYTRACER_PLANE_HPP
