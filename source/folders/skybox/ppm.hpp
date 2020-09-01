//
// Created by Jacob Lammert on 01.09.2020.
//

#ifndef RAYTRACER_PPM_HPP
#define RAYTRACER_PPM_HPP


#include <string>
#include <glm-0.9.5.3/glm/vec3.hpp>
#include <vector>

struct Ppm {
    Ppm();

    void load(const std::string& filename);

    glm::vec3 get_pixel(glm::vec2 position) const;

    int width_ = 0;
    int height_ = 0;

    std::vector<std::vector<glm::vec3 >> image_;

};


#endif //RAYTRACER_PPM_HPP
