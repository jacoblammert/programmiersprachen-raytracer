#include "scene.hpp"


Scene::Scene(std::map<std::string,std::shared_ptr<Material>> material_map,
             std::map<std::string,std::shared_ptr<Shape>> shape_map,
             std::map<std::string,std::shared_ptr<Light>> light_map,
             std::map<std::string,std::shared_ptr<Camera>> camera_map,
             glm::vec3 ambient):

    material_map {material_map},
    shape_map {shape_map},
    light_map {light_map},
    camera_map {camera_map},
    ambient {ambient}
{}

Scene::drawScene(Camera camera, /* filename, */ int x_res, int y_res) const {
    
    //TODO calculate with given resulution
    unsigned const image_width = 1920  /* 2 */ /  2; //640
    unsigned const image_height = 1080 /* 2 */ / 2; //360
    Window window{{image_width, image_height}};
    
    
    
}
