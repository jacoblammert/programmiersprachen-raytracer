#define CATCH_CONFIG_RUNNER

#include <catch.hpp>
#include "prog5/Sphere.h"
#include "prog5/Box.h"
#include "prog5/Shape.h"
#include <cmath>

#define _USE_MATH_DEFINES

#include <math.h>

/**
 * 5.2
 */
TEST_CASE("area + volume", "[area, volume]") {
/**/
    Sphere sphere{{0, 0, 0}, -7,"",{}};

    REQUIRE(sphere.volume() == Approx((4 * M_PI * 7 * 7 * 7)/3));
    REQUIRE(sphere.area() == Approx(4 * M_PI * 7));

    sphere = {{999, 999, -999}, -7,"",{}};

    REQUIRE(sphere.volume() == Approx((4 * M_PI * 7 * 7 * 7)/3));
    REQUIRE(sphere.area() == Approx(4 * M_PI * 7));

    sphere = {{999, 999, -999}, 0,"",{}};

    REQUIRE(sphere.volume() == Approx(0));
    REQUIRE(sphere.area() == Approx(0));

    sphere = {{999, 999, -999}, -0,"",{}};

    REQUIRE(sphere.volume() == Approx(0));
    REQUIRE(sphere.area() == Approx(0));




    Box box{{0, 0, 0},
            {1, 1, 1},"",{}};

    REQUIRE(box.area() == 6);
    REQUIRE(box.volume() == 1);

    box = {{-10, -10, -10},
           {10,  10,  10},"",{}};

    REQUIRE(box.area() == 2400);
    REQUIRE(box.volume() == 8000);


    box = {{10, 10, 10},
           {-10,  -10, -10},"",{}};

    REQUIRE(box.area() == 2400);
    REQUIRE(box.volume() == 8000);

    box = {{0, 0, 0},
           {0,  0, 0},"",{}};

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

    Sphere sphere{{0, 0, 0}, -7,"NAME",{0,0,0}};
    sphere = {{999, 999, -999}, 0,"",{8,7777,0.4}};
    sphere = {{999, 999, -999}, -0,"wow, wie soll ich den Konstruktor testen??",{0,0,0}};

    Box box{{0, 0, 0},{1, 1, 1},"NAME",{0,0,0}};
    box = {{-10, -10, -10},{10,  10,  10},"",{8,7777,0.4}};
    box = {{10, 10, 10},{-10,  -10, -10},"wow, wie soll ich den Konstruktor testen??",{0,0,0}};
}
/**
 * 5.4
 * print
 *
 */

TEST_CASE("print test", "[print test]") {


    Sphere sphere{{0, 0, 0}, -7,"NAME",{0,0,0}};
    sphere.print(std::cout);
    sphere = {{999, 999, -999}, 0,"",{8,7777,0.4}};
    sphere.print(std::cout);
    sphere = {{999, 999, -999}, -0,"wow, wie soll ich den Konstruktor testen??",{0,0,0}};
    sphere.print(std::cout);

    Box box{{0, 0, 0},{1, 1, 1},"NAME",{0,0,0}};
    box.print(std::cout);
    box = {{-10, -10, -10},{10,  10,  10},"",{8,7777,0.4}};
    box.print(std::cout);
    box = {{10, 10, 10},{-10,  -10, -10},"wow, wie soll ich den Konstruktor testen??",{0,0,0}};
    box.print(std::cout);

}

int main(int argc, char *argv[]) {
    return Catch::Session().run(argc, argv);
}
