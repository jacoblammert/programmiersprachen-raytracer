#ifndef RAYTRACER_BOX_HPP
#define RAYTRACER_BOX_HPP

#include <cmath>
#include <iostream>
#include <vector>

#include "shape.hpp"

class Box : public Shape {
public:
    Box();

    Box (std::string const& name, const glm::vec3 &minXminYminZ, const glm::vec3 &maxXmaxYmaxZ);
    
    Box (const glm::vec3 &minXminYminZ, const glm::vec3 &maxXmaxYmaxZ);

    Box(glm::vec3 const& pos, float xScale, float yScale, float zScale);

    bool get_intersect_vec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    void get_intersect_vec(glm::vec3 &direction,  glm::vec3 &HitNormal) const; /// für Skybox

    bool get_intersect(const Ray &ray) const;

    glm::vec3 get_normal(glm::vec3 const& pos) const override;

    glm::vec3 get_min() const override;

    glm::vec3 get_max() const override;

    glm::vec3 get_median() const override;

    std::shared_ptr<Material> get_material() override;

    void set_material (std::shared_ptr<Material> const& material) override;

    void print() const override;
    
    void translate(glm::vec3 const& position) override;

private:
    int sign (glm::vec3 const& vec3, int position) const;

    std::vector<glm::vec3> bounds_;
};


#endif //RAYTRACER_BOX_HPP
