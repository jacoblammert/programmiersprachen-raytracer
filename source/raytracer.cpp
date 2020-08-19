#include <cmath>
#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <omp.h>
#include "folders/loader/sdfLoader.hpp"
#include "folders/camera/camera.hpp"
#include "folders/shapes/sphere.hpp"
#include "folders/shapes/box.hpp"

//TODO add folders for shapes, loading, saving,...


int main(int argc, char *argv[]) {

    SdfLoader loader{"../../source/folders/sdfFiles/start.sdf"};
    loader.loadFile();



    unsigned const image_width = 800;
    unsigned const image_height = 600;
    std::string const filename = "./checkerboard.ppm";

    Renderer renderer{image_width, image_height, filename};

    renderer.render();

    Window window{{image_width, image_height}};

    Camera camera{{5,0,0},{-1,0,0},image_width,image_height,0.1};

    //Sphere sphere{{0,0,0},2};
    Box sphere{{0,0,0},1,1,1};

    float stepsize = 0.1f;
    float step = 0;

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }

        //camera.lookAt({std::sin(step),std::cos(step),0});

        step += stepsize;

        std::cout<<step<<std::endl;


        omp_set_num_threads(4); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren
#pragma omp parallel for

        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten
            for (int j = 0; j < image_height; ++j) {
                Pixel color{(unsigned int)i,(unsigned int)j};
                Ray ray = camera.generateRay((int)i,(int)j);


                glm::vec3 notusefullrightnow;
                float dist = INFINITY-1; // will later be set to closest distance where the ray intersected an object



                if (sphere.getIntersectVec(ray,notusefullrightnow,notusefullrightnow,dist)) {
                    color.color = {1, 0, 0};
                } else{
                    color.color = {1, 1, 1};
                }
                renderer.write(color);
            }
        }



        window.show(renderer.color_buffer());
    }

    return 0;
}
