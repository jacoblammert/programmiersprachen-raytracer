#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

#include <map>
//#include <omp.h>
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

class Scene {
    
public:
    Scene(std::shared_ptr<Composite> composite,
          std::map<std::string,std::shared_ptr<Light>> light_map,
          std::map<std::string,std::shared_ptr<Camera>> camera_map,
          const glm::vec3& ambient);
    
    void draw_scene(Camera camera, std::string filename, unsigned int x_res, unsigned int y_res) const;
    
private:
    std::shared_ptr<Composite> composite_;
    std::map<std::string,std::shared_ptr<Light>> light_map_;
    std::map<std::string,std::shared_ptr<Camera>> camera_map_;
    glm::vec3 ambient_;
};


#endif //RAYTRACER_SCENE_HPP
