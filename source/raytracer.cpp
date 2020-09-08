
#include <memory>
//#include <omp.h>
#include "folders/loader/sdfLoader.hpp"
#include "folders/loader/sdfwriter.hpp"

//TODO add folders for saving,...


int main(int argc, char *argv[]) {


    int frames = 10;//24 * 10; // 24 frames per second -> 10s long Animation

    std::string animation_name = "test";


    /// Creates all the sdf files needed
    /// If we want to change anything in the animation (add a material/ Shape/ Change the resolution) we can do it in the original (start.sdf) file
    SdfLoader loader{"../../source/folders/sdfFiles/start.sdf"};
    loader.load_file();





    std::shared_ptr<Scene> scene = loader.get_scene();

    float progress = 0;

    for (int i = 0; i < frames; ++i) {

        progress = (float)i/(float)frames;

        float camera_distance = 4;
        scene->cameras_[0]->set_position(glm::vec3{camera_distance * sin(progress * 2 * MATH_PI),3,camera_distance * cos(progress * 2 * MATH_PI)}); // The Camera rotates around the origin with a height of 4
        scene->cameras_[0]->look_at(glm::vec3{0,0,0}); /// The Camera looks at the origin



        /// Example of editing the scenes content. The reflectivity varies from 0 to 1
        //scene->materials_[1]->glossy = (float) (1 + sin(progress * 2.0f * MATH_PI))/2;


        SdfWriter sdf_writer(animation_name + std::to_string(i) + ".sdf");
        sdf_writer.create_sdf(scene);
    }



    /// loads all the written sdf files and renders the images
    for (int i = 0; i < frames; ++i) {


        loader = {"../../source/folders/sdfFiles/" + animation_name + std::to_string(i) + ".sdf"};
        loader.load_file();

        scene = loader.get_scene();

        scene->antialiasing_samples_ = 1;

        scene->draw_frame(0,"../../source/folders/images/"+animation_name + std::to_string(i) + ".ppm"); /// Renders the loaded sdf file and saves it as ppm
    }


    //scene->draw_scene(0, "image.ppm"); // Maybe open a window after it rendered? With the information of the last .sdf file 

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
 - Ambiente der Szene einlesen
 - Beobachter ist im Ursprung und blickt entlang negativer z-Achse
 - Kegel und Zylinder hinzufügen (optional) // nur die Standart Kegel/ Zylinder in Ursprung mit Größe 1 funktionieren -> Zylinder + Kegel (inverted translations/ rotations)
 - einlesen einer Szene im SDF-Format und rendern der Szene
 - (etwas schwerer) andere Objekte (Triangle, Plane, Cone, Cylinder) einlesen (wichtig!)
 - (machbar) Kameramodell im Parser erweitern nur fov - brauchen wir noch mehr?
 - (machbar) Anti-Aliasing, Interpolation (optional) 4 mal so großes Bild berechnen + kleiner machen und Durchschnitt der 4 Farbwerte pro Pixel berechnen
 - (machbar) .ppm Dateien in Ordner speichern -> zu Animation verarbeiten
 - (etwas schwerer) Translation, Rotation, Skalierung hinzufügen (updated min_max_mid functions) (So gut wie möglich/ teilweise)

 nicht fertig:
 - (machbar) Animation aus gerenderten Einzelbildern erstellen (Programm das für jeden Frame eine SDF-Datei generiert)  (Mit der Animation kann nun begonnen werden)
 - zeichnen auch ohne composite ermöglichen (Arbeitsblatt Beipspiel SDF)
 - composite mit nur einem Element nicht möglich
 - objekte müssen eindeutige namen haben
 - mehrere Composites einlesen möglich?

 
 Anmerkungen:
 - eventuell shared pointer als const möglich? z.B. std::shared_ptr <Shape const>
 - const correctness der Methoden + Parameter
 - default initialisierung structs
 - floats, ... initialisieren
 - Kommentare
 - Formatierung, Namensgebung (struct variablen ohne _)
 - reihenfolge methoden in hpp = cpp
 - Konsistenz: light hpp Konstruktorliste in cpp
 - alle includes otwendig?
 */
