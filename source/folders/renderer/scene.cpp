//#include <omp.h>
#include "scene.hpp"

Scene::Scene(std::shared_ptr<Composite> composite,
             std::map<std::string, std::shared_ptr<Light>> light_map,
             std::map<std::string, std::shared_ptr<Camera>> camera_map,
             const glm::vec3& ambient) :
        composite_{std::move(composite)},
        light_map_{std::move(light_map)},
        camera_map_{std::move(camera_map)},
        ambient_{ambient} {}

void Scene::draw_scene(Camera camera, std::string filename, unsigned int x_res, unsigned int y_res) const {

    unsigned const image_width = x_res;
    unsigned const image_height = y_res;

    Window window{{x_res, y_res}};

    PpmWriter ppm_writer(x_res, y_res, filename);

    std::vector<std::shared_ptr<Light>> lights;

    for (auto it = light_map_.begin(); it != light_map_.end(); it++) {
        lights.push_back(it->second);
    }

    Renderer renderer{x_res, y_res,filename};
    camera.set_width_hight((int) image_width * 2, (int) image_height * 2); // * 2 for antialiasing


    Render render;
    composite_->print();
    render.set_composite(composite_);
    render.set_ambient_scene(ambient_);
    render.set_lights(lights);


    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        camera.set_direction(window);
        camera.move(window);

        float start_time = window.get_time();


//        omp_set_num_threads(128);
//#pragma omp parallel for
        for (int i = 0; i < image_width; ++i) {
            for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};


                glm::vec3 color_vec;
                for (int k = 0; k < 4; ++k) {


                    glm::vec3 color_vec_1 = render.get_color(camera.generate_ray(2 * i + k % 2, 2 * j + (int)(floor( k/2))), 0);

                    color_vec += color_vec_1 / (color_vec_1 + glm::vec3{1, 1, 1});


                }
                color_vec /= 4;
                color.color = {color_vec[0], color_vec[1], color_vec[2]};

                renderer.write(color);
                ppm_writer.write(color);
            }
        }


        window.show(renderer.color_buffer());
        std::cout << "Time: " << round((window.get_time() - start_time) * 100) / 100 << " Fps: "
                  << round(100 / (window.get_time() - start_time)) / 100 << std::endl;
    }
    ppm_writer.save(filename);


}
