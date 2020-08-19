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
#include "folders/shapes/triangle.hpp"
#include "folders/shapes/plane.hpp"

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

    Camera camera{{5,5,5},{-1,0,0},image_width,image_height,5};

    //Sphere object{{0,0,0},2}; // kaputt
    Box object{{0,0,0},2,2,2}; // geht
    //Triangle object{{2,0,0},{0,2,0},{0,0,2}}; // geht
    //Plane object{{2,0,0},{0,0,1}}; // geht, vielleicht invertiert / falsch herum?

    int stepsize = 1;
    int step = 1;

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }

        //step += stepsize;
        //std::cout<<step<<std::endl;
        //camera.setPosition({std::sin(step) * 5.0f,std::cos(step) * 5.0f,2});

        camera.lookAt({});
        camera.print(); // for changing positions, but the view doesent update, therefore it is useless right now



//        omp_set_num_threads(4); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren
//#pragma omp parallel for

        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten
            for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int)i,(unsigned int)j};
                Ray ray = camera.generateRay(i,j);

                glm::vec3 notusefullrightnow;
                float dist = INFINITY-1; // will later be set to closest distance where the ray intersected an object

                if (object.getIntersectVec(ray,notusefullrightnow,notusefullrightnow,dist)) {
                    color.color = {1, 0, 0}; // shape is red
                } else{
                    color.color = {1, 1, 1}; // shape has not been hit
                }

                renderer.write(color);
            }
        }
        window.show(renderer.color_buffer());
        //window.update();
    }

    return 0;
}
