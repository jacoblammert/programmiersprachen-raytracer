#ifndef RAYTRACER_RENDER_HPP
#define RAYTRACER_RENDER_HPP


#include <vector>
#include <color.hpp>
#include <memory>
#include "../camera/ray.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/composite.hpp"
#include "light.hpp"

class Render {
public:
    Render();

    glm::vec3 get_color(Ray const& ray, int depth) const;

    void set_composite(std::shared_ptr<Composite> const& composite1);

    void set_lights(std::vector<std::shared_ptr<Light>> const& lights1);


private:

    glm::vec3 get_refracted_color(Ray const& ray, glm::vec3 const& HitPoint, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape) const;

    glm::vec3 get_reflected_color(Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape) const;

    float random_float() const;

    glm::vec3 get_refracted(const glm::vec3& vector, glm::vec3 const& normal, float n) const;

    glm::vec3 get_reflected(const glm::vec3& vector, glm::vec3 const& normal) const;

    std::shared_ptr<Composite> composite_;
    std::vector<std::shared_ptr<Light>> lights_;
};


#endif //RAYTRACER_RENDER_HPP
