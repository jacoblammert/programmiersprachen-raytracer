
#ifndef RAYTRACER_PPM_HPP
#define RAYTRACER_PPM_HPP


#include <string>
#include <glm-0.9.5.3/glm/vec3.hpp>
#include <vector>

struct Ppm {
    Ppm();

    void load(const std::string& filename);

    glm::vec3 get_pixel(glm::vec2 position) const;

    int width = 0;
    int height = 0;

    std::vector<std::vector<glm::vec3 >> image;

};


#endif //RAYTRACER_PPM_HPP
