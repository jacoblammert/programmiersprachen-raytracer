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
#include "folders/shapes/cone.hpp"
#include "folders/shapes/cylinder.hpp"

//TODO add folders for saving,...


int main(int argc, char *argv[]) {

    SdfLoader loader{"../../source/folders/sdfFiles/start.sdf"};
    loader.load_file();

    
    //unsigned const image_width = 1920  /* 2 */ /  2; //640
    //unsigned const image_height = 1080 /* 2 */ / 2;//360
    //std::string const filename = "./checkerboard.ppm";
/*
    Window window{{image_width, image_height}};

    ///old materials:
    std::shared_ptr<Material> white;// = std::make_shared<Material>(Material{0,0,0,1,{1,1,1}});
    std::shared_ptr<Material> transparent;// = std::make_shared<Material>(Material{0,0,1,1.36f,{1,1,1}});
    std::shared_ptr<Material> mirror;// = std::make_shared<Material>(Material{0,1,0,1,{1,1,1}});

    ///new materials:
    white = std::make_shared<Material>(Material{{1,1,1},{0,1,0},{1,1,1},10.0f});
    transparent = std::make_shared<Material>(Material{{1,1,1},{1,0,0},{1,1,1},10.0f});
    mirror = std::make_shared<Material>(Material{{1,1,1},{0,0,1},{1,1,1},10.0f});
    mirror->glossy_ = 1;
    transparent->refractive_index_ = 1.36;
    transparent->opacity_ = 1.0f;

    std::vector<std::shared_ptr<Shape>> shapes;
    for (int i = 0; i < 20; ++i) {
        float x = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float y = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        float z = ((float) (rand() % 10000) / 5000) - 1; // number between -1 and 1
        glm::vec3 position{x, y, z};
        position *= 5;
        if (i % 2 == 1) {
            shapes.push_back(std::make_shared<Sphere>(Sphere{position, 0.512636125}));
            shapes[i]->set_material(transparent);
        } else {
            shapes.push_back(std::make_shared<Box>( Box{position, 0.25f, 0.25f, 0.25f}));
            shapes[i]->set_material(white);
        }
    }


    //shapes.push_back(std::make_shared<Cone>(Cone{{2,0,-2},{1,0,0},5,1})); /// Cone ray intersection not working correctly
    //shapes[shapes.size()-1]->set_material(white);

    //shapes.push_back(std::make_shared<Cylinder>(Cylinder{{2,0,-2},{1,0,0},5,5})); /// Cone ray intersection not working correctly
    //shapes[shapes.size()-1]->set_material(white);

    //shapes.push_back(std::make_shared<Sphere>(Sphere{{2,0,0}, 1}));
    //shapes[shapes.size()-1]->set_material(white);

    //shapes.push_back(std::make_shared<Plane>(Plane{{0, 0, -8},{0, 0, 1}}));


    shapes.push_back(std::make_shared<Triangle>(Triangle{{-10, 10, -8},{10, -10, -8},{10, 10, -8}}));
    shapes[shapes.size()-1]->set_material(white);
    shapes.push_back(std::make_shared<Triangle>(Triangle{{10, -10, -8},{-10, 10, -8},{-10, -10, -8}}));
    shapes[shapes.size()-1]->set_material(white);




    shapes.push_back(std::make_shared<Box>( Box{{0,0,-6.95}, 4, 2, 2}));
    shapes[shapes.size()-1]->set_material(mirror);


    //TODO if it loads really slowly or not at all, pr its running way to slow, try activating a few lines in the cmake file and set the mode from default or debug to release


    std::vector<std::shared_ptr<Light>> lights;

    lights.push_back(std::make_shared<Light>(Light{{0, 0, -5}, {1, 1, 1}, {20.0f, 20.0f, 20.0f},1})); //brightness vec3
    //lights.push_back(std::make_shared<Light>(Light{{0, 0, 5}, {1, 1, 1}, 11}));

    std::shared_ptr<Composite> composite = std::make_shared<Composite>(Composite{shapes});


    float step_size = 0.01f;
    float step = 0;
    Renderer renderer {image_width, image_height, filename};


    Camera camera{{-5, -5, -5}, {0, 0, -1}, image_width, image_height, 1}; // old, no fov
    //Camera camera{{5, 5, 5}, image_width, image_height, 60}; // new, with fov

    bool pause = false;
    float pause_time = window.get_time();

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }

        camera.set_direction(window);
        camera.move(window);

        /// The pause button (left click) can only be pressed in a given intervall
        if (window.get_mouse_button(0) == 1 && 0.3f < (window.get_time() - pause_time)){
            pause = !pause;
            pause_time = window.get_time();
        }

        if (!pause) {
            step += step_size;

            shapes[shapes.size()-4]->set_rotation_axis({cos(step),cos(step),sin(step)});
        }

        //step = window.get_time() * 0.3;
        
        float start_time = window.get_time();

        lights[0]->position_ = {0.5*std::cos(step/3), 0.25*std::sin(step/3), -3.5 + std::sin(step/3)};
        lights[0]->position_ = {3 * std::cos(3 * step), 3 * std::sin(2 * step), 7 * std::cos( 0.75 * step)};
        lights[0]->brightness_ = {1 + std::cos(3 * step), 1 + std::sin(2 * step), 1 + std::cos(0.75 * step)};
        lights[0]->brightness_ = lights[0]->brightness_ * 10.0f;

        //lights[0]->color = {1,1,1};


        //lights[1]->position = {3 * std::sin(1.7 * step), 3 * std::cos(3.4 * step), 5 * std::cos( 1.5*0.75 * step)};
        //lights[1]->color = {lights[0]->color[2],1-lights[0]->color[0],1-lights[0]->color[1]};

        /// The color of the light ranges from 0 to 1
/

        /// Depth of field effect:
        /// The focal point is the middle of the screen (white dot)

        /// Does not work to well with close points

        float dist = INFINITY;
        glm::vec3 hit_point;
        glm::vec3 hit_normal;
        std::shared_ptr<Shape> shape = nullptr;
        camera.set_depth_of_Field(0,1);

        float x = image_width * 0.5f;
        float y = image_height * 0.5f;
        composite->get_intersected_shape(camera.generate_ray(x,y),shape,hit_point,hit_normal,dist);

        camera.set_depth_of_Field(dist,dist);
/*




//        omp_set_num_threads(128); //TODO falls das nicht gehen sollte, einfach diese beiden Zeilen auskommentieren + das in CMake.txt
//#pragma omp parallel for

        for (int i = 0; i < image_width; ++i) {
            // kein Code hier, sonnst kann es nicht parallel arbeiten

            Render render;
            render.set_composite(composite);
            render.set_lights(lights);

        for (int j = 0; j < image_height; ++j) {

                Pixel color{(unsigned int) i, (unsigned int) j};

                glm::vec3 color_vec = render.get_color(camera.generate_ray(i, j), 0);

                color_vec = color_vec/ (color_vec + glm::vec3{1,1,1});

                color.color = {color_vec[0], color_vec[1], color_vec[2]};

                renderer.write(color);
            }
        }

        /// Point in the middle of the screen
        Pixel color{(unsigned int) image_width/2, (unsigned int) image_height/2};
        color.color = {1,1,1};
        renderer.write(color);



        window.show(renderer.color_buffer());

        std::cout << "Time: " << round((window.get_time() - start_time) * 100)/100 << " Fps: " << round(100/(window.get_time() - start_time))/100<< std::endl;
    }*/

    return 0;
}
/* checkerboard pattern: // könnte man vielleicht auch für ein Material nutzen
                    int x = positionvec[0] < 0 ? positionvec[0] - 1 : positionvec[0];
                    int y = positionvec[1] < 0 ? positionvec[1] - 1 : positionvec[1];
                    x /=5;
                    y /=5;
                    if ((x + y) % 2) {
                        color.color = {1, 1, 1};
                    } else {
                        color.color = {};
                    }
*/

/*
 TODO:
 fertig:
 - Szene kann aus bliebig vielen Objekten bestehen - eine Szene kommt in ein composite objekt (mit shapes und Lichtern & kleineren Comosite objekten)
 - mindestens achsenparalleler Quader, Kugeln
 - objekt kann ein Material haben
 - Objekte werfen Schatten
 - Spiegelung hinzufügen (optional)
 - Refraktion hinzufügen (optional)
 - Composite Pattern hinzufügen, Parser erweitern
 - Kameramodell aus der Vorlesung mit fov
 - Dreieck
 - Kegel und Zylinder hinzufügen (optional) /// funktionieren zwar, können aber nicht gedreht werden (Transparenz/ Reflektion für Kegel ist sehr langsam & nicht korrekt)
 - Beobachter ist im Ursprung und blickt entlang negativer z-Achse - nur mit Kameraconstruktor No. 2 mit fov
 - Material Werte/Typen anpassen auf Vorgaben
 - Szene hat beliebig viele Punktlichtquellen -> ja (in der Vorlesung vorgestelltes Beleuchtungsmodell) - phong shading-> fehlt noch
 - finaler Farbwert wird berechnet und im Window angezeigt/ in der Ausgabedatei gespeichert -> framework sollte das schon können. Falls nicht, habe ich auch noch eine alte Klasse um .ppm Dateien zu speichern

 nicht fertig:
 - Ambiente der Szene einlesen
 - einlesen einer Szene im SDF-Format und rendern der Szene
 - Beobachter ist im Ursprung und blickt entlang negativer z-Achse - nur mit Kameraconstruktor No. 2 mit fov   (y muss oben sein)
 - Translation, Rotation, Skalierung hinzufügen (updated min_max_mid functions)
 - Kameramodell im Parser erweitern
 - Animation aus gerenderten Einzelbildern erstellen (Programm das für jeden Frame eine SDF-Datei generiert online suchen)
 - .ppm Dateien in Ordner speichern -> zu Animation verarbeiten
 - Kegel und Zylinder hinzufügen (optional) // nur die Standart Kegel/ Zylinder in Ursprung mit Größe 1 funktionieren -> Zylinder + Kegel (inverted translations/ rotations)
 - Anti-Aliasing, Interpolation (optional) (mehrere Strahlen oder Interpolation der Pixel nach Berechnung der Farbwerte?)

 
 Anmerkungen:
 - eventuell shared pointer als const möglich? z.B. std::shared_ptr <Shape const>
 - const correctness der Methoden
 - default initialisierung structs
 */
