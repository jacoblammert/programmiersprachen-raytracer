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
#include "../shapes/sphere.hpp"
#include "../shapes/composite.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/material.hpp"

class Scene { // Vielleicht als struct/ DTO?
    
public:
    Scene();
    Scene(std::vector<std::shared_ptr<Material>> materials,
            std::shared_ptr<Composite> composite,
          std::vector<std::shared_ptr<Light>> lights,
          std::vector<std::shared_ptr<Camera>> cameras,
          const glm::vec3& ambient,
          int x_res, int y_res);

    void draw_scene(int camera, const std::string& filename) const;
    void draw_frame(int camera, const std::string& filename) const;
    
//private:

    std::vector<std::shared_ptr<Material>> materials_;
    std::shared_ptr<Composite> composite_;
    std::vector<std::shared_ptr<Light>> lights_;
    std::vector<std::shared_ptr<Camera>> cameras_;
    glm::vec3 ambient_;
    int antialiasing_samples_ = 1;
    int x_res_ = 0;
    int y_res_ = 0;

};


#endif //RAYTRACER_SCENE_HPP
