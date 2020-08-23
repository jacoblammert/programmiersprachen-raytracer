#include <cmath>
#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <memory>
//#include <omp.h>
#include "folders/loader/sdfLoader.hpp"
#include "folders/camera/camera.hpp"
#include "folders/shapes/sphere.hpp"
#include "folders/shapes/box.hpp"
#include "folders/shapes/triangle.hpp"
#include "folders/shapes/plane.hpp"
#include "folders/shapes/composite.hpp"
#include "folders/renderer/render.hpp"

//TODO add folders for saving,...


int main(int argc, char *argv[]) {

    SdfLoader loader{"../../source/folders/sdfFiles/start.sdf"};
    loader.loadFile();


    unsigned const image_width = 1920  /*/ * 2/**/ /  2; //640
    unsigned const image_height = 1080 /*/ * 2/**/ / 2;//360
    std::string const filename = "./checkerboard.ppm";


    Window window{{image_width, image_height}};

    std::vector<std::shared_ptr<Shape>> object; // usefull later on

    object.push_back(std::make_shared<Sphere>(Sphere{{0, 0, 0}, 2}));
    object.push_back(std::make_shared<Box>(Box{{0, 0, 0}, 2, 2, 2}));
    object.push_back(std::make_shared<Triangle>(Triangle{{2, 0, 0},
                                                         {0, 2, 0},
                                                         {0, 0, 2}}));
    object.push_back(std::make_shared<Plane>(Plane{{2, 0, 0},
                                                   {0, 0, 1}}));


    std::vector<std::shared_ptr<Shape>> shapes;
    for (int i = 0; i < 50; ++i) {
        float x = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float y = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float z = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        glm::vec3 position{x, y, z};
        position *= 5;
        if (i % 2 == 1) {
        shapes.push_back(std::make_shared<Sphere>(Sphere{position, 0.13512636125}));
        } else {
            shapes.push_back(std::make_shared<Box>( Box{position, 0.125f, 0.125f, 0.125f}));
        }
    }

    shapes.push_back(std::make_shared<Plane>(Plane{{0, 0, 8},{0, 0, 1}}));


    //shapes.push_back(std::make_shared<Triangle>(Triangle{{-10, 10, 10},{10, -10, 10},{10, 10, 10}}));
    //shapes.push_back(std::make_shared<Triangle>(Triangle{{0, 0, -8},{0, 0, 1},{0, 0, 1}}));
    //shapes.push_back(std::make_shared<Triangle>(Triangle{{0, 0, -8},{0, 0, 1},{0, 0, 1}}));
    //shapes.push_back(std::make_shared<Triangle>(Triangle{{0, 0, -8},{0, 0, 1},{0, 0, 1}}));


    std::vector<std::shared_ptr<Light>> lights;
    lights.push_back(std::make_shared<Light>(Light{{0, 0, 5}, {1, 1, 1}, 5}));
    //lights.push_back(std::make_shared<Light>(Light{{0, 0, 5}, {1, 1, 1}, 8}));

    std::shared_ptr<Composite> composite = std::make_shared<Composite>(Composite{shapes});


    //std::vector<std::shared_ptr<Composite>> compositevector;
    //compositevector.push_back(composite);

    float stepsize = 0.1f;
    float step = 0;
    Renderer renderer{image_width, image_height, filename};


    Camera camera{{5, 5, 5}, {-1, 0, 0}, image_width, image_height, 1};

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }

        float starttime = window.get_time();



        float cameradistance = 15; //5.0f

        step += stepsize;
        step = starttime;
        //camera.setPosition(
        //        {std::sin(step) * cameradistance, std::cos(step) * cameradistance,
        //         2 * std::cos(0.75 * step)});


        glm::vec2 mouse = window.mouse_position();


        if ( 0 < mouse[0] && mouse[0] < image_width && 0 < mouse[1] && mouse[1] < image_height) {


            float mouseX = mouse[0] / image_width;
            float mouseY = mouse[1] / image_height;

            float x = 5 * sin(3.14f * 2 * (mouseX));
            float y = 5 * cos(3.14f * 2 * (mouseX));
            float z = cos(3.14f *  (mouseY));

            x *= (1 - abs(z));
            y *= (1 - abs(z));
            z *= -5;

            camera.setPosition({0,0,0});

            camera.lookAt({ x, 0+y, z});
        }



        lights[0]->position = {3 * std::cos(3 * step), 3 * std::sin(2 * step), 7 * std::cos( 0.75 * step)};
        //lights[1]->position = {3 * std::sin(1.7 * step), 3 * std::cos(3.4 * step), 5 * std::cos( 1.5*0.75 * step)};
        lights[0]->color = {1 + std::cos(3 * step), 1 + std::sin(2 * step), 1 + std::cos(0.75 * step)};
        lights[0]->color = lights[0]->color * 0.5f;
        //lights[1]->color = {lights[0]->color[2],1-lights[0]->color[0],1-lights[0]->color[1]};
        /// The color of the light ranges from 0 to 1


//        omp_set_num_threads(64); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren + das in CMake.txt
//#pragma omp parallel for

        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten

            Render render;
            render.setComposite(composite);
            render.setLights(lights);

        for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};

                glm::vec3 colorveec = render.getColor(camera.generateRay(i, j), 0);

                color.color = {colorveec[0], colorveec[1], colorveec[2]};

                renderer.write(color);


            }
        }
        window.show(renderer.color_buffer()); // leider wird es nicht jedes mal geupdated, wenn es aufgerufen wird

        std::cout<<"Time: " << window.get_time() - starttime << std::endl;
    }

    return 0;
}
/*////checkerboard pattern:
                    int x = positionvec[0] < 0 ? positionvec[0] - 1 : positionvec[0];
                    int y = positionvec[1] < 0 ? positionvec[1] - 1 : positionvec[1];
                    x /=5;
                    y /=5;
                    if ((x + y) % 2) {
                        color.color = {1, 1, 1};
                    } else {
                        color.color = {};
                    }
                    /**/