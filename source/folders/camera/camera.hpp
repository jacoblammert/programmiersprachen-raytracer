//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#define MATH_PI 3.14159265359

#include <glm-0.9.5.3/glm/geometric.hpp>
#include <cmath>
#include <iostream>
#include <window.hpp>
#include "../camera/ray.hpp"
//#include "../math/Vector.h"


class Camera {
public:
    Camera(glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance);

    Camera(glm::vec3 const& position, int width, int height, float angle, std::string const& name); //TODO implement this function

    Ray generate_ray(int x, int y) const;
    void set_position(glm::vec3 const& pos);
    void translate(glm::vec3 const& pos);
    void look_at(glm::vec3 const& pos);
    void print() const;

    void move(Window const& window);
    void set_direction(Window const& window);

    //void translate(glm::vec3 const& position);

private:
    glm::vec3 position_;  /// could be put together with direction to generate a ray with pos/ direction
    glm::vec3 direction_; // normalized
    int width_;  // of image
    int height_; // of image
    float distance_; // distance from cameraposition to cameraplane
    glm::vec3 up_vector_ {0, 0, 1};
};


#endif //RAYTRACER_CAMERA_H
