#ifndef RAYTRACER_CYLINDER_HPP
#define RAYTRACER_CYLINDER_HPP


#include "shape.hpp"
#include <cmath>

class Cylinder: public Shape {
    
public:
    Cylinder (std::string const& name, const glm::vec3& position,const glm::vec3& axis,float width, float height);

    bool get_intersect_vec(Ray const &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const override;

    glm::vec3 get_normal(glm::vec3 const &pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

    std::shared_ptr<Material> get_material() override;

    void set_material(std::shared_ptr<Material> const &material) override;

    void translate(glm::vec3 const &position) override;

    std::string get_information()const override;

private:
    float height_;
    float width_;
};


#endif //RAYTRACER_CYLINDER_HPP
