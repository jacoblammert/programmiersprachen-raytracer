//
// Created by Jacob Lammert on 01.09.2020.
//

#ifndef RAYTRACER_SKYBOX_HPP
#define RAYTRACER_SKYBOX_HPP


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <renderer.hpp>
#include "../shapes/box.hpp"

struct Skybox {
public:
    Skybox();
    glm::vec3 get_color(glm::vec3 direction) const;

    void set_images(Renderer renderer);
    unsigned int width_ = 410;
    unsigned int height_ = 410;
private:

    Renderer top_=    Renderer{width_,height_,"../../source/folders/skybox/posy.ppm"}; // "../../source/folders/sdfFiles/start.sdf"
    Renderer bottom_= Renderer{width_,height_,"../../source/folders/skybox/negy.ppm"}; // "../../source/folders/skybox/"
    Renderer left_=   Renderer{width_,height_,"../../source/folders/skybox/negx.ppm"};
    Renderer right_=  Renderer{width_,height_,"../../source/folders/skybox/posx.ppm"};
    Renderer front_=  Renderer{width_,height_,"../../source/folders/skybox/negz.ppm"};
    Renderer back_=   Renderer{width_,height_,"../../source/folders/skybox/posz.ppm"};

    Box skybox_ = {{0, 0, 0}, 2, 2, 2};

    glm::vec3 get_pixel(int x, int y,Renderer const & renderer) const;
    glm::vec3 get_pixel_interpolated(float x, float y,Renderer const & renderer) const;
    glm::vec3 interpolate(const glm::vec3& A,const glm::vec3& B, float value) const;
};


#endif //RAYTRACER_SKYBOX_HPP
