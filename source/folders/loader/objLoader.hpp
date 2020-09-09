#ifndef RAYTRACER_OBJLOADER_H
#define RAYTRACER_OBJLOADER_H


#include <vector>
#include "../shapes/triangle.hpp"

class objLoader {
public:
    objLoader();
    std::vector<std::shared_ptr<Shape>> load_obj(std::string filename);
};


#endif //RAYTRACER_OBJLOADER_H
