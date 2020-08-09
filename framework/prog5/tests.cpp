#define CATCH_CONFIG_RUNNER

#include <catch.hpp>
#include "prog5/Sphere.h"
#include "prog5/Box.h"
#include "prog5/Shape.h"
#include <cmath>

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

#include <math.h>

//TODO ignore all the code below


/**
 * 5.2
 */
TEST_CASE("area + volume", "[area, volume]") {
/**/
    Sphere sphere{{0, 0, 0}, -7, "", {}};

    REQUIRE(sphere.volume() == Approx((4 * M_PI * 7 * 7 * 7) / 3));
    REQUIRE(sphere.area() == Approx(4 * M_PI * 7));

    sphere = {{999, 999, -999}, -7, "", {}};

    REQUIRE(sphere.volume() == Approx((4 * M_PI * 7 * 7 * 7) / 3));
    REQUIRE(sphere.area() == Approx(4 * M_PI * 7));

    sphere = {{999, 999, -999}, 0, "", {}};

    REQUIRE(sphere.volume() == Approx(0));
    REQUIRE(sphere.area() == Approx(0));

    sphere = {{999, 999, -999}, -0, "", {}};

    REQUIRE(sphere.volume() == Approx(0));
    REQUIRE(sphere.area() == Approx(0));


    Box box{{0, 0, 0},
            {1, 1, 1}, "", {}};

    REQUIRE(box.area() == 6);
    REQUIRE(box.volume() == 1);

    box = {{-10, -10, -10},
           {10, 10, 10}, "", {}};

    REQUIRE(box.area() == 2400);
    REQUIRE(box.volume() == 8000);


    box = {{10, 10, 10},
           {-10, -10, -10}, "", {}};

    REQUIRE(box.area() == 2400);
    REQUIRE(box.volume() == 8000);

    box = {{0, 0, 0},
           {0, 0, 0}, "", {}};

    REQUIRE(box.area() == 0);
    REQUIRE(box.volume() == 0);
/**/
}
/** 5.3
 *
 *
 * Die Initialisierung der Basisklasse erfolgt in der Initialisierungsliste
 * des Konstruktors der abgeleiteten Klasse! Wo wird der Konstruktor der
 * Basisklasse aufgerufen?
 *
 *
 * Box::Box(glm::vec3 min, glm::vec3 max, std::string name, Color color):
 * max_(max),min_(min),
 *
 * Shape(name,color) // Hier wird der Konstruktor der Basisklasse aufgerufen.
 *
 * {
 *
 * }

 */
TEST_CASE("name + color", "[name, color]") {

    /**
     * If the Constructors work, this test will not crash, if they
     * do not work, it will crash
     */

    Sphere sphere{{0, 0, 0}, -7, "NAME", {0, 0, 0}};
    sphere = {{999, 999, -999}, 0, "", {8, 7777, 0.4}};
    sphere = {{999, 999, -999}, -0, "wow, wie soll ich den Konstruktor testen??", {0, 0, 0}};

    Box box{{0, 0, 0}, {1, 1, 1}, "NAME", {0, 0, 0}};
    box = {{-10, -10, -10}, {10, 10, 10}, "", {8, 7777, 0.4}};
    box = {{10, 10, 10}, {-10, -10, -10}, "wow, wie soll ich den Konstruktor testen??", {0, 0, 0}};
}
/**
 * 5.4
 * print
 *
 */

TEST_CASE("print test", "[print test]") {


    Sphere sphere{{0, 0, 0}, -7, "NAME", {0, 0, 0}};
    sphere.print(std::cout);
    sphere = {{999, 999, -999}, 0, "", {8, 7777, 0.4}};
    sphere.print(std::cout);
    sphere = {{999, 999, -999}, -0, "wow, wie soll ich den Konstruktor testen??", {0, 0, 0}};
    sphere.print(std::cout);

    Box box{{0, 0, 0}, {1, 1, 1}, "NAME", {0, 0, 0}};
    box.print(std::cout);
    box = {{-10, -10, -10}, {10, 10, 10}, "", {8, 7777, 0.4}};
    box.print(std::cout);
    box = {{10, 10, 10}, {-10, -10, -10}, "wow, wie soll ich den Konstruktor testen??", {0, 0, 0}};
    box.print(std::cout);

}

/**
 * 5.5
 *
 * Erklären Sie den Effekt des Schlüsselworts override im Kontext der Vererbung!
 * Benutzen Sie das Schlüsselwort dementsprechend in Ihrer Methodendeklaration.
 * Was passiert, wenn Sie das Schlüsselwort weglassen?
 *
 * es wird eine neue Methode gleichen namens erstellt
 *
 * [12 Punkte]
 * Hinweis: Um den Namen und die Farbe auszugeben, sollte die Methode
 * Shape::print explizit in der überschriebenen Methode aufgerufen werden
 *
 * Durch die Funktion override kann eine schon deklarierte Methode aus einer Klasse in den abgeleiteten Klassen
 * für diese angepasst werden.
 * z.B eine Print methode für Sphere kann die print methode der Shape klasse überschreiben,
 * da eine Sphere noch zusätzlich einen Radius und ein Center besitzt.
 *
 */

///5.6

TEST_CASE (" intersect_ray_sphere ", "[intersect]") {
// Ray
    glm::vec3 ray_origin{0.0f, 0.0f, 0.0f};
// ray direction has to be normalized !
// you can use :
// v = glm :: normalize ( some_vector )
    glm::vec3 ray_direction{0.0f, 0.0f, 1.0f};
// Sphere
    glm::vec3 sphere_center{0.0f, 0.0f, 5.0f};

    float sphere_radius{1.0f};
    float distance = 0.0f;
    auto result = glm::intersectRaySphere(
            ray_origin, ray_direction,
            sphere_center,
            sphere_radius * sphere_radius, // squared radius !!!
            distance);


    REQUIRE (distance == Approx(4.0f));


    Sphere a{{0, 0, 5}, 1, "Sphere", {1, 0, 0}};
    Ray ray{{0, 0, 0},
            {0, 0, 1}};

    HitPoint hitPoint = a.intersect(ray);

    REQUIRE (hitPoint.hit_);
    REQUIRE (hitPoint.dist_ == Approx(4.0f));
    REQUIRE (hitPoint.name_ == "Sphere");
    REQUIRE (hitPoint.color_.r == 1);
    REQUIRE (hitPoint.color_.g == 0);
    REQUIRE (hitPoint.color_.b == 0);

    ///Ray inside Sphere

    glm::vec3 rayOriginTEST{0,0,0};
    glm::vec3 rayDirectionTEST{1,0,0};
    glm::vec3 spherePositionTEST{0,0,0};
    float radiusTEST = 2;


    result = glm::intersectRaySphere(
            rayOriginTEST, rayDirectionTEST,
            spherePositionTEST,
            radiusTEST * radiusTEST, // squared radius !!!
            distance);



    a={spherePositionTEST, radiusTEST, "Sphere", {1, 0, 0}};
    ray={rayOriginTEST,
         rayDirectionTEST};

    hitPoint = a.intersect(ray);

    REQUIRE (fabsf(hitPoint.dist_) == fabsf(distance));
    REQUIRE (result == hitPoint.hit_);

    ///Ray inside Sphere, but the radius is 0

    rayOriginTEST = {0,0,0};
    rayDirectionTEST = {1,0,0};
    spherePositionTEST = {0,0,0};
    radiusTEST = 0;


    result = glm::intersectRaySphere(
            rayOriginTEST, rayDirectionTEST,
            spherePositionTEST,
            radiusTEST * radiusTEST, // squared radius !!!
            distance);



    a={spherePositionTEST, radiusTEST, "Sphere", {1, 0, 0}};
    ray={rayOriginTEST,
         rayDirectionTEST};

    hitPoint = a.intersect(ray);

    REQUIRE (hitPoint.dist_ == distance);
    REQUIRE (result == hitPoint.hit_);



    ///Ray behind Sphere, but the radius is 0

    rayOriginTEST = {-0.1,0,0};
    rayDirectionTEST = {1,0,0};
    spherePositionTEST = {1,0,0};
    radiusTEST = 0;


    result = glm::intersectRaySphere(
            rayOriginTEST, rayDirectionTEST,
            spherePositionTEST,
            radiusTEST * radiusTEST, // squared radius !!!
            distance);



    a={spherePositionTEST, radiusTEST, "Sphere", {1, 0, 0}};
    ray={rayOriginTEST,
         rayDirectionTEST};

    hitPoint = a.intersect(ray);

    REQUIRE (hitPoint.dist_ == distance);
    REQUIRE (result == hitPoint.hit_);


}


///  5.7
TEST_CASE (" static dynamic ", "[static dynamic]") {
    std::cout<<"\n\n\nAufgabe5.7\n\n\n";
    Color red {255 , 0, 0};
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    std::shared_ptr <Sphere > s1 = std::make_shared <Sphere >( position , 1.2f, " sphere0 ", red );
    std::shared_ptr <Shape > s2 = std::make_shared <Sphere >( position , 1.2f,  " sphere1 ", red );
    s1->print(std::cout);
    s2->print(std::cout);
}


/** 5.7
 *
 * Color red {255 , 0, 0};
 *
 * glm::vec3 position {0.0f, 0.0f, 0.0f};
 *
 * std::shared_ptr <Sphere > s1 = std::make_shared <Sphere >( position , 1.2f, red , " sphere0 ");
 *
 * std::shared_ptr <Shape > s2 = std::make_shared <Sphere >( position , 1.2f, red , " sphere1 ");
 *
 * s1->print(std::cout);
 *
 * s2->print(std::cout);
 *
 *
 *
 *
 *
 * Erklären Sie anhand des Beispiels die Begriffe „Statischer Typ einer Variablen“
 * und „Dynamischer Typ einer Variablen“.
 *
 *
 *
 *
 *
 * Wann wird welche Art des Typs überprüft?
 *
 * Die Art des Types wird während der Übersetztungszeit überprüft
 *
 *
 *
 *
 * Was sind die dynamischen und die
 * statischen Typen der Variablen s1 bzw s2?
 *
 * Die Dynamischen Typen sind Sphere bei s1 und s2, Die statischen sind Sphere bei s1 und Shape bei s2
 *
 */


///5.8


TEST_CASE (" virtual destructor ", "[destructor]") {
    std::cout<<"Aufgabe 5.8:\n\n\n\n";
    Color red{255 , 0, 0};
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    Sphere * s1 = new Sphere{ position , 1.2f, " sphere0 ",red};
    Shape * s2 = new Sphere{ position , 1.2f,  " sphere1 ",red};

    s1 -> print (std::cout );
    s2 -> print (std::cout );
    delete s1;
    delete s2;

    /**
     * Zuerst wird der Shape Constructor aufgerufen und danach der Sphere Constructor
     *
     * Beim Destructor wird zuerst der der Sphere aufgerufen und danach der Destructor der Shape-Klasse
     *
     * Mit virtual:
     *
     * Sphere Destructor
     * Shape Destructor
     * Sphere Destructor
     * Shape Destructor
     *
     * Fehlt das virtual in der Shape Klasse, so felht der Sphere destructor Aufruf des Shape pointers
     *
     * ohne virtual:
     *
     * Sphere Destructor
     * Shape Destructor
     * Shape Destructor
     *
     * Der Destructor kann nicht überladen werden, wenn das virual fehlt, und daher wird beim Shape* nicht mehr überprüft,
     * ob eine Überladung/ implementierung existiert
     */

}

/**
 *
 * Aufgabe 5.9
 * Erklären Sie die Unterschiede zwischen Klassenhierarchie vs. Objekthierarchie -
 * Klassendiagramm vs. Objektdiagramm.
 *
 * Die Klassenhiererchie beschreibt den Zusammenhang verschiederner Klassen im Sinne der Implementierung
 *
 * z.B. Shape,               ---> Basisklasse
 * Sphere - erbt von Shape , ---> Abgeleitete Klasse
 * Box - erbt von Shape,     ---> Abgeleitete Klasse
 * Composite - erbt von Shape---> Abgeleitete Klasse
 *
 * Aber Composite kann Sphere + Box enthalten + Drawmethode, die die Drawmethode von Box + Sphere nutzt
 *
 * Shape setzt sich aus Composite zusammen
 *
 * */


int main(int argc, char *argv[]) {
    return Catch::Session().run(argc, argv);
}
