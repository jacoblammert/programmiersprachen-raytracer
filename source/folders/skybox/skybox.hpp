#ifndef RAYTRACER_SKYBOX_HPP
#define RAYTRACER_SKYBOX_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <renderer.hpp>
#include "../shapes/box.hpp"
#include "ppm.hpp"

struct Skybox {
public:
    Skybox();
    glm::vec3 get_color(glm::vec3 direction) const;

private:

    Ppm top_;
    Ppm bottom_;
    Ppm left_;
    Ppm right_;
    Ppm front_;
    Ppm back_;

    Box skybox_ = {{0, 0, 0}, 2, 2, 2};

    glm::vec3 get_pixel_interpolated(float x, float y,Ppm const & image) const;
    glm::vec3 interpolate(const glm::vec3& A,const glm::vec3& B, float value) const;
};


#endif //RAYTRACER_SKYBOX_HPP
