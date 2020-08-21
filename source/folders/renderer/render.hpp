//
// Created by Jacob Lammert on 21.08.2020.
//

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

    glm::vec3 getColor(Ray ray, int depth) const;

    void setComposite(std::shared_ptr<Composite> composite1);

    void setLights(std::vector<std::shared_ptr<Light>> lights1);


private:

    bool castShadowRay(const Ray& ray, float distance) const;

    glm::vec3 getRefractedColor(Ray ray, glm::vec3 HitPoint, glm::vec3 HitNormal, int depth, std::shared_ptr<Shape> shape) const;

    glm::vec3 getReflectedColor(Ray ray, glm::vec3 HitPoint, glm::vec3 HitNormal, int depth, std::shared_ptr<Shape>) const;

    float randomFloat(float range) const;



    glm::vec3 getRefracted(const glm::vec3& vector, const glm::vec3& normal, float n) const;

    glm::vec3 getReflected(const glm::vec3& vector, glm::vec3 normal) const;

    std::shared_ptr<Composite> composite;
    std::vector<std::shared_ptr<Light>> lights;
};


#endif //RAYTRACER_RENDER_HPP
