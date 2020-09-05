//#include <omp.h>
#include "scene.hpp"

Scene::Scene(std::shared_ptr<Composite> composite,
             std::vector<std::shared_ptr<Light>> lights,
             std::vector<std::shared_ptr<Camera>> cameras,
             const glm::vec3& ambient) :
        composite_{std::move(composite)},
        lights_{std::move(lights)},
        cameras_{std::move(cameras)},
        ambient_{ambient} {}

void Scene::draw_scene(Camera camera, std::string filename, unsigned int x_res, unsigned int y_res) const {

    unsigned const image_width = x_res;
    unsigned const image_height = y_res;

    Window window{{x_res, y_res}};

    PpmWriter ppm_writer(x_res, y_res, filename);

    Renderer renderer{x_res, y_res,filename};


    int samples = 4; /// 4 Samples per Pixel
    samples = 1; // no antialiasing (only one sample per Pixel)


    camera.set_width_height((int) image_width * sqrt(samples), (int) image_height * sqrt(samples)); // * 2 for antialiasing


    Render render;
    //composite_->print();
    render.set_composite(composite_);
    render.set_ambient_scene(ambient_);
    render.set_lights(lights_);


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
                for (int k = 0; k < samples; ++k) {


                    glm::vec3 color_vec_1 = render.get_color(camera.generate_ray(sqrt(samples) * i + k % (int)sqrt(samples), sqrt(samples) * j + (int)(floor( k/sqrt(samples)))), 0);

                    color_vec += color_vec_1 / (color_vec_1 + glm::vec3{1, 1, 1});


                }
                color_vec /= samples;
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
    
    SdfWriter sdf_writer ("Hallo.sdf");
    sdf_writer.create_sdf(composite_, lights_, cameras_, ambient_);

}
