#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

//#include <omp.h>
#include <utility>
#include <renderer.hpp>
#include <ppmwriter.hpp>
#include <window.hpp>
#include "../camera/camera.hpp"
#include "light.hpp"
#include "render.hpp"
#include "sdfwriter.hpp"
#include "../shapes/sphere.hpp"
#include "../shapes/composite.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/material.hpp"

class Scene {
    
public:
    Scene(std::shared_ptr<Composite> composite,
          std::vector<std::shared_ptr<Light>> lights,
          std::vector<std::shared_ptr<Camera>> cameras,
          const glm::vec3& ambient);
    
    void draw_scene(Camera camera, std::string filename, unsigned int x_res, unsigned int y_res) const;
    
private:
    std::shared_ptr<Composite> composite_;
    std::vector<std::shared_ptr<Light>> lights_;
    std::vector<std::shared_ptr<Camera>> cameras_;
    glm::vec3 ambient_;
};


#endif //RAYTRACER_SCENE_HPP
