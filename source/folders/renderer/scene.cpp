#include "scene.hpp"
#include <omp.h>
#include <iostream>
#include "../shapes/sphere.hpp"

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

    unsigned const image_width = x_res;
    unsigned const image_height = y_res;
    
    Window window {{image_width, image_height}};
    
    PpmWriter ppm_writer (x_res, y_res, filename);
    
    //TODO use given composite - besserer Weg möglich als maps zu vector? -> composite aus sdfLoader übernehmen
    
    std::vector<std::shared_ptr<Shape>> shapes;

    for (auto it = shape_map.begin(); it != shape_map.end(); it++) {
        //shapes.push_back(it->second);
    }
/**/
///Zufällige shapes funktionieren, die Eingelesenen sind viel größer!! (100 mal ca.)und haben bei mir zumindest noch nicht ganz funktioniert
    for (int i = 0; i < 30; ++i) {
        float x = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float y = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float z = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        glm::vec3 position{x, y, z};
        position *= 50;

        auto white = std::make_shared<Material>(Material{{1,1,1},{0,1,0},{1,1,1},10.0f});
        if (i % 2 == 1) {
            shapes.push_back(std::make_shared<Sphere>(Sphere{position, 5.12636125}));
            shapes[i]->set_material(white);
        } else {
            shapes.push_back(std::make_shared<Box>( Box{position, 2.5f, 2.5f, 2.5f}));
            shapes[i]->set_material(white);
        }
    }/**/
    
    std::shared_ptr<Composite> composite = std::make_shared<Composite>(Composite{shapes});
    
    std::vector<std::shared_ptr<Light>> lights;
    
    for (auto it = light_map.begin(); it != light_map.end(); it++) {
         lights.push_back(it->second);
     }
    
    Renderer renderer {image_width, image_height, filename};
    camera.set_width_hight((int)image_width,(int)image_height);


    Render render;
    render.set_composite(composite);
    render.set_lights(lights);


     while (!window.should_close()) {
            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            camera.set_direction(window);
         camera.move(window);

         float start_time = window.get_time();


//        omp_set_num_threads(128); //TODO versuch bitte nochmal omp zum laufen zu bringen, damit kann es viel schneller die Bilder anzeigen und es ist weniger frustrierend auf die neuen Ergebnisse zu warten
//#pragma omp parallel for

            for (int i = 0; i < image_width; ++i) {
                // kein Code hier, sonnst kann es nicht parallel arbeiten


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
         std::cout << "Time: " << round((window.get_time() - start_time) * 100)/100 << " Fps: " << round(100/(window.get_time() - start_time))/100<< std::endl;
    }
    ppm_writer.save(filename);
}
