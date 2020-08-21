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
#include "folders/shapes/composite.hpp"

//TODO add folders for saving,...


int main(int argc, char *argv[]) {

    SdfLoader loader{"../../source/folders/sdfFiles/start.sdf"};
    loader.loadFile();


    unsigned const image_width = 1920 / 2; //640
    unsigned const image_height = 1080 / 2;//360
    std::string const filename = "./checkerboard.ppm";


    Window window{{image_width, image_height}};


    //TODO clean up this mess later
    std::vector<std::shared_ptr<Shape>> object; // usefull later on

    object.push_back(std::shared_ptr<Shape> (new Sphere{{0, 0, 0}, 2}));
    object.push_back(std::shared_ptr<Shape> (new Box{{0, 0, 0}, 2, 2, 2}));
    object.push_back(std::shared_ptr<Shape> (new Triangle{{2, 0, 0},
                                  {0, 2, 0},
                                  {0, 0, 2}}));
    object.push_back(std::shared_ptr<Shape> (new Plane{{2, 0, 0},
                               {0, 0, 1}}));


    std::vector<std::shared_ptr<Shape>> shapes;
    for (int i = 0; i < 80; ++i) {
        float x = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float y = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float z = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        glm::vec3 position{x, y, z};
        position *= 5;
        //if (i % 2 == 1) {
        shapes.push_back(std::shared_ptr<Shape> (new Sphere{position, 0.2636125}));
        //} else {
        //    shapes.push_back(new Box{position, 01.125f, 01.125f, 01.125f});
        //}
    }

    shapes.push_back(std::shared_ptr<Shape> (new Plane{{0, 0, 0},{0, 0, 1}}));

    Composite composite{shapes};


    float stepsize = 0.1f;
    float step = 0;
    Renderer renderer{image_width, image_height, filename};

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }


        Camera camera{{5, 5, 5}, {-1, 0, 0}, image_width, image_height, 1};

        float cameradistance = 20; //5.0f

        step += stepsize;
        camera.setPosition(
                {std::sin(step) * cameradistance, std::cos(step) * cameradistance,
                 2 * std::cos(0.75 * step)});

        camera.lookAt({});


//       omp_set_num_threads(128); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren + das in CMake.txt
//#pragma omp parallel for

        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten
            for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};
                Ray ray = camera.generateRay(i, j);
                //TODO everything in here can be put in a render class / function
                glm::vec3 normalvec;
                glm::vec3 positionvec;
                float dist = INFINITY - 1; // will later be set to closest distance where the ray intersected an object

                std::shared_ptr<Shape> shape; // will later be used in a different class to get the refractive index/ material properties
                bool hit = false;




                composite.getIntersectedShape(ray, shape, positionvec, normalvec, dist,hit); //TODO simplify function to take a new pointer as parameter and reduce the amount of data send for each ray, even if it will not hit anything

                /// hit gets changed in the function above

                if (hit) {

                    normalvec *= positionvec;

                    normalvec += glm::vec3{1, 1, 1};
                    normalvec *= 0.5f;

                    color.color = {normalvec[0], normalvec[1], normalvec[2]};


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
                } else {
                    color.color = {0, 0, 0}; // shape has not been hit
                }

                renderer.write(color);
            }
            //window.show(renderer.color_buffer()); /// malt jede Zeile einzeln
        }
        //renderer = {image_width, image_height, filename}; /// malt jede Zeile einzeln + schwarzer Hintergrund für neues Bild
        window.show(renderer.color_buffer()); // leider wird es nicht jedes mal geupdated, wenn es aufgerufen wird
    }

    return 0;
}
