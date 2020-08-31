#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

#include <map>

#include <renderer.hpp>
#include <window.hpp>
#include "../shapes/shape.hpp"
#include "../shapes/material.hpp"
#include "../camera/camera.hpp"
#include "light.hpp"
#include "render.hpp"

class Scene {
    
public:
    Scene(/*std::map<std::string,std::shared_ptr<Material>> material_map,*/          std::map<std::string,std::shared_ptr<Shape>> shape_map, std::map<std::string,std::shared_ptr<Light>> light_map, std::map<std::string,std::shared_ptr<Camera>> camera_map,
        glm::vec3 ambient);
    
    void draw_scene(Camera camera, std::string filename, int x_res, int y_res) const;
    
private:
    //std::map<std::string,std::shared_ptr<Material>> material_map;
    std::map<std::string,std::shared_ptr<Shape>> shape_map;
    std::map<std::string,std::shared_ptr<Light>> light_map;
    std::map<std::string,std::shared_ptr<Camera>> camera_map;
    glm::vec3 ambient;
};


#endif //RAYTRACER_SCENE_HPP
