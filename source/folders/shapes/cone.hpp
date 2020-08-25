#ifndef RAYTRACER_CONE_HPP
#define RAYTRACER_CONE_HPP


#include "shape.hpp"

class Cone: public Shape {
public:
    Cone(const glm::vec3& position,const glm::vec3& direction,float angle, float height);

    bool get_intersect_vec(Ray const &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const override;

    glm::vec3 get_normal(glm::vec3 const &pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

    std::shared_ptr<Material> get_material() override;

    void set_material(std::shared_ptr<Material> const &material) override;

    void print() const override;

    void translate(glm::vec3 const &position) override;

private:
    glm::vec3 pos_; // bottom of the cone
    glm::vec3 axis_;
    float height_;
    float angle_;

};
#endif //RAYTRACER_CONE_HPP
