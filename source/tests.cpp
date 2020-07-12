#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "prog5/Sphere.h"
#include "prog5/Box.h"
#include "prog5/Shape.h"


TEST_CASE("area + volume","[area, volume]"){

    Sphere sphere{{0,0,0},-7};
    Box box{{0,0,0},{1,1,1}};

    REQUIRE(box.area() == 6);
    REQUIRE(box.volume() == 1);

    box = {{-10,-10,-10},{10,10,10}};

    REQUIRE(box.area() == 6* 400);
    REQUIRE(box.volume() == 8000);

    //TODO add more tests for sphere + negative volume, volume = 0, a few random values
}


int main(int argc, char *argv[]){
    //TODO add more tests
  return Catch::Session().run(argc, argv);
}
