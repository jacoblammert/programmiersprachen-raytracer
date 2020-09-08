//#include <omp.h>
#include "scene.hpp"
#include <utility>

Scene::Scene(std::vector<std::shared_ptr<Material>> materials,
             std::shared_ptr<Composite> composite,
             std::vector<std::shared_ptr<Light>> lights,
             std::vector<std::shared_ptr<Camera>> cameras,
             const glm::vec3 &ambient,
             int x_res, int y_res) :
        materials_{std::move(materials)},
        composite_{std::move(composite)},
        lights_{std::move(lights)},
        cameras_{std::move(cameras)},
        ambient_{ambient},
        x_res_{x_res}, y_res_{y_res} {

}

Scene::Scene() {

}
/**
 * opens a window and displays the rendered image on it
 * It saves the last rendered image as a ppm file with the given string
 * @param camera 0 if only one camera has been read by the sdfLoader
 * @param name_image "image.ppm" as example
 */
void Scene::draw_scene(int camera, const std::string &name_image) const {


    Window window{{x_res_, y_res_}};

    PpmWriter ppm_writer(x_res_, y_res_, name_image);

    Renderer renderer{(unsigned int) x_res_, (unsigned int) y_res_, name_image};


    cameras_[camera]->set_width_height((int) (x_res_ * sqrt(antialiasing_samples_)),
                                       (int) (y_res_ * sqrt(antialiasing_samples_))); // * 2 for antialiasing


    Render render;
    render.set_composite(composite_);
    render.set_ambient_scene(ambient_);
    render.set_lights(lights_);


    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        cameras_[camera]->set_direction(window);
        cameras_[camera]->move(window);

        float start_time = window.get_time();

//        omp_set_num_threads(128);
//#pragma omp parallel for
        for (int i = 0; i < x_res_; ++i) {
            for (int j = 0; j < y_res_; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};


                glm::vec3 color_vec;
                for (int k = 0; k < antialiasing_samples_; ++k) {


                    glm::vec3 color_vec_1 = render.get_color(cameras_[camera]->generate_ray(
                            (int) (sqrt(antialiasing_samples_) * i + k % (int) sqrt(antialiasing_samples_)),
                            (int) (sqrt(antialiasing_samples_) * j + (int) (floor(k / sqrt(antialiasing_samples_))))),
                                                             0);

                    color_vec += color_vec_1 / (color_vec_1 + glm::vec3{1, 1, 1});


                }
                color_vec /= antialiasing_samples_;
                color.color = {color_vec[0], color_vec[1], color_vec[2]};

                renderer.write(color);
                ppm_writer.write(color);
            }
        }


        window.show(renderer.color_buffer());
        std::cout << "Time: " << round((window.get_time() - start_time) * 100) / 100 << " Fps: "
                  << round(100 / (window.get_time() - start_time)) / 100 << std::endl;
    }
    ppm_writer.save(name_image);
}

/**
 * Renders only a single image and saves it as a ppm file with the given string
 * @param camera 0 if only one camera has been read by the sdfLoader
 * @param name_image "image.ppm" as example
 */
void Scene::draw_frame(int camera, const std::string &name_image) const {

    PpmWriter ppm_writer(x_res_, y_res_, name_image);

    Renderer renderer{(unsigned int) x_res_, (unsigned int) y_res_, name_image};


    cameras_[camera]->set_width_height((int) (x_res_ * sqrt(antialiasing_samples_)),
                                       (int) (y_res_ * sqrt(antialiasing_samples_)));


    Render render;
    render.set_composite(composite_);
    render.set_ambient_scene(ambient_);
    render.set_lights(lights_);


//        omp_set_num_threads(128);
//#pragma omp parallel for
    for (int i = 0; i < x_res_; ++i) {
        for (int j = 0; j < y_res_; ++j) {

            Pixel color{(unsigned int) i, (unsigned int) j};


            glm::vec3 color_vec;
            for (int k = 0; k < antialiasing_samples_; ++k) {


                glm::vec3 color_vec_1 = render.get_color(cameras_[camera]->generate_ray(
                        (int) (sqrt(antialiasing_samples_) * i + k % (int) sqrt(antialiasing_samples_)),
                        (int) (sqrt(antialiasing_samples_) * j + (int) (floor(k / sqrt(antialiasing_samples_))))), 0);
                color_vec += color_vec_1 / (color_vec_1 + glm::vec3{1, 1, 1});
            }
            color_vec /= antialiasing_samples_;
            color.color = {color_vec[0], color_vec[1], color_vec[2]};

            renderer.write(color);
            ppm_writer.write(color);
        }
    }
    ppm_writer.save(name_image);
}
