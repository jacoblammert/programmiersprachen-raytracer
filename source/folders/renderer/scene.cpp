#include "scene.hpp"

#include <iostream>

Scene::Scene(//std::map<std::string,std::shared_ptr<Material>> material_map,
             std::map<std::string,std::shared_ptr<Shape>> shape_map,
             std::map<std::string,std::shared_ptr<Light>> light_map,
             std::map<std::string,std::shared_ptr<Camera>> camera_map,
             glm::vec3 ambient):

    //material_map {material_map},
    shape_map {shape_map},
    light_map {light_map},
    camera_map {camera_map},
    ambient {ambient}
{}

void Scene::draw_scene(Camera camera, std::string filename, int x_res, int y_res) const {
    
    //TODO calculate with given resulution
    //unsigned const image_width = 1920  /* 2 */ /  2; //640
    //unsigned const image_height = 1080 /* 2 */ / 2; //360
    unsigned const image_width = x_res;
    unsigned const image_height = y_res;
    
    Window window {{image_width, image_height}};
    
    PpmWriter ppm_writer (x_res, y_res, filename);
    
    //TODO use given composite - besserer Weg möglich als maps zu vector?
    
    std::vector<std::shared_ptr<Shape>> shapes;
    
    for (auto it = shape_map.begin(); it != shape_map.end(); it++) {
        shapes.push_back(it->second);
    }
    
    std::shared_ptr<Composite> composite = std::make_shared<Composite>(Composite{shapes});
    
    std::vector<std::shared_ptr<Light>> lights;
    
    for (auto it = light_map.begin(); it != light_map.end(); it++) {
         lights.push_back(it->second);
     }
    
    Renderer renderer {image_width, image_height, filename};
    
    
     while (!window.should_close()) {
            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            camera.set_direction(window);

            for (int i = 0; i < image_width; ++i) {
                // kein Code hier, sonnst kann es nicht parallel arbeiten

                Render render;
                render.set_composite(composite);
                render.set_lights(lights);

            for (int j = 0; j < image_height; ++j) {

                    Pixel color {(unsigned int) i, (unsigned int) j};

                    glm::vec3 color_vec = render.get_color(camera.generate_ray(i, j), 0);

                    color_vec = color_vec/ (color_vec + glm::vec3{1,1,1});

                    color.color = {color_vec[0], color_vec[1], color_vec[2]};

                    renderer.write(color);
                
                    ppm_writer.write (color);
                }
            }

            window.show(renderer.color_buffer());
    }
    ppm_writer.save(filename);
}
