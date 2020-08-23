//
// Created by Jacob on 10.08.2020.
//

#ifndef RAYTRACER_SDFLOADER_H
#define RAYTRACER_SDFLOADER_H


#include <string>
#include <vector>
#include <glm-0.9.5.3/glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <sstream> // string stream -> easy parsing mechanics
#include <string>
#include <utility>
#include <map>
#include "../shapes/composite.hpp"
#include "../shapes/sphere.hpp"

class SdfLoader {
public:
    SdfLoader(std::string filepath);

    void load_file() const;

private:

    std::string filepath_;
};


#endif //RAYTRACER_SDFLOADER_H
