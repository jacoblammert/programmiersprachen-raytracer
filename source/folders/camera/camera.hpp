#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#define MATH_PI 3.14159265359

#include <glm-0.9.5.3/glm/geometric.hpp>
#include <cmath>
#include <iostream>
#include <window.hpp>
#include "../camera/ray.hpp"
//#include "../math/Vector.h"


class Camera {
public:
    Camera (float fov_x);
    Camera (float fov_x, glm::vec3 const& eye, glm::vec3 const& direction, glm::vec3 const& up);
    Camera(glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance);

    Camera(glm::vec3 const& position, int width, int height, float fov);

    Ray generate_ray(int x, int y) const;
    void set_position(glm::vec3 const& pos);
    void set_width_hight(int width, int height);
    void translate(glm::vec3 const& pos);
    void look_at(glm::vec3 const& pos);
    void print() const;

    void move(Window const& window);
    void set_direction(Window const& window);

    void set_depth_of_Field(float doF_strength, float focal_length);
    glm::vec3 get_position()const;

    //void translate(glm::vec3 const& position);

private:
    float random_float() const;

    glm::vec3 position_ = glm::vec3{0,0,0};  /// could be put together with direction to generate a ray with pos/ direction
    glm::vec3 direction_ = glm::vec3{0,0,-1}; // normalized  // change upvec then change 0.000001, 0.000001 back to 0,0 because cross does not work, if the area is zero
    int width_;  // of image
    int height_; // of image
    float distance_; // distance from cameraposition to cameraplane
    glm::vec3 up_vector_ {0,1, 0};
    float doF_strength_ = 0;
    float focal_length_ = 1;
};


#endif //RAYTRACER_CAMERA_HPP
