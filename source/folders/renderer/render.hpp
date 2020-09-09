#ifndef RAYTRACER_RENDER_HPP
#define RAYTRACER_RENDER_HPP


#include <vector>
#include <color.hpp>
#include <memory>
#include <cmath>
#include "../camera/ray.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/composite.hpp"
#include "../skybox/skybox.hpp"
#include "light.hpp"

class Render {
public:
    glm::vec3 get_color (Ray const& ray, int depth) const;
    void set_composite (std::shared_ptr<Composite> const& composite1);
    void set_lights (std::vector<std::shared_ptr<Light>> const& lights1);
    void set_ambient_scene (glm::vec3 const& ambient);
    
    Skybox skybox_ = Skybox();
    
private:
    glm::vec3 get_brightness_color (Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape) const;
    glm::vec3 get_refracted_color (Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape) const;
    glm::vec3 get_reflected_color (Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape) const;
    float random_float() const;
    glm::vec3 get_refracted_vec3 (const glm::vec3& vector, glm::vec3 const& normal, float n) const;
    glm::vec3 get_reflected_vec3 (const glm::vec3& vector, glm::vec3 const& normal) const;

    std::shared_ptr<Composite> composite_;
    std::vector<std::shared_ptr<Light>> lights_;
    glm::vec3 ambient_scene_;

};


#endif //RAYTRACER_RENDER_HPP
