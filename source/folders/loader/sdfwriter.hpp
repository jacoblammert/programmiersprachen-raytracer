#ifndef RAYTRACER_SDFWRITER_HPP
#define RAYTRACER_SDFWRITER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include "../renderer/scene.hpp"
#include "../camera/camera.hpp"
#include "../renderer/light.hpp"
#include "../renderer/render.hpp"
#include "../shapes/sphere.hpp"
#include "../shapes/composite.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/material.hpp"

class SdfWriter {
    
public:
    SdfWriter (std::string const& file);
    void create_sdf (std::shared_ptr<Scene> const& scene);
    
private:
    std::string filename_;
    std::string file_;

};


#endif //RAYTRACER_SDFWRITER_HPP
