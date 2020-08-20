#include <cmath>
#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
//#include <omp.h>
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


    unsigned const image_width = 1920 / 3; //640
    unsigned const image_height = 1080 / 3;//360
    std::string const filename = "./checkerboard.ppm";


    Window window{{image_width, image_height}};


    Sphere object{{0, 0, 0}, 2};
    //Box object{{0,0,0},2,2,2};
    //Triangle object{{2, 0, 0},{0, 2, 0},{0, 0, 2}};
    //Plane object{{2,0,0},{0,0,1}};

    float stepsize = 0.3f;
    float step = 0;

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }




        Camera camera{{5, 5, 5}, {-1, 0, 0}, image_width, image_height, 2};

        step += stepsize;
        //std::cout << step << std::endl;
        camera.setPosition({std::sin(step) * 5.0f, std::cos(step) * 5.0f, 2});

        camera.lookAt({});
        //camera.print(); // for changing positions, but the view doesent update, therefore it is useless right now



//        omp_set_num_threads(64); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren
//#pragma omp parallel for
        Renderer renderer{image_width, image_height, filename};
        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten
            for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};
                Ray ray = camera.generateRay(i, j);

                glm::vec3 normalvec;
                float dist = INFINITY - 1; // will later be set to closest distance where the ray intersected an object

                if (object.getIntersectVec(ray, normalvec, normalvec, dist)) {

                    normalvec += glm::vec3{1,1,1};
                    normalvec *= 0.5f;

                    color.color = {normalvec[0], normalvec[1],
                                   normalvec[2]};//{1, 0, 0}; // shape is red
                } else {
                    color.color = {1, 1, 1}; // shape has not been hit
                }

                renderer.write(color);
            }
        }

        window.show(renderer.color_buffer()); // leider wird es nicht jedes mal geupdated, wenn es aufgerufen wird
    }

    return 0;
}
