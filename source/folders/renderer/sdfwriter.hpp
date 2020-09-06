#ifndef RAYTRACER_SDFWRITER_HPP
#define RAYTRACER_SDFWRITER_HPP

//#include <omp.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../camera/camera.hpp"
#include "light.hpp"
#include "render.hpp"
#include "../shapes/sphere.hpp"
#include "../shapes/composite.hpp"
#include "../shapes/shape.hpp"
#include "../shapes/material.hpp"

class SdfWriter {
    
public:
    SdfWriter (std::string const& file);
    
    void create_sdf (std::vector<std::shared_ptr<Material>> materials,
            std::shared_ptr<Composite> composite,
    std::vector<std::shared_ptr<Light>> lights_,
    std::vector<std::shared_ptr<Camera>> cameras_,
                                glm::vec3 ambient_) const;
    
private:
    std::string file_;
};


#endif //RAYTRACER_SDFWRITER_HPP
