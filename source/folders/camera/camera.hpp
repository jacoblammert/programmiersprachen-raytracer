#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#define MATH_PI 3.14159265359

#include <glm-0.9.5.3/glm/geometric.hpp>
#include <cmath>
#include <math.h>
#include <utility>
#include <iostream>
#include <string>
#include <window.hpp>
#include "../camera/ray.hpp"


class Camera {
public:
    Camera (std::string const& name, float fov_x);
    Camera (std::string const& name, float fov_x, glm::vec3 const& eye, glm::vec3 const& direction, glm::vec3 const& up);
    Camera (glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance);
    Camera (glm::vec3 const& position, int width, int height, float fov);

    Ray generate_ray (int x, int y) const;
    void set_position (glm::vec3 const& pos);
    void set_width_height (int width, int height);
    void set_direction (Window const& window);
    void set_depth_of_field (float dof_strength, float focal_length);
    glm::vec3 get_position() const;
    std::string get_name() const;
    std::string get_information() const;
    void print() const;
    void translate (glm::vec3 const& pos);
    void look_at (glm::vec3 const& pos);
    void move (Window const& window);

private:
    float random_float() const;

    std::string name_;
    float fov_x_;
    // could be put together with direction to generate a ray with pos/ direction
    glm::vec3 position_ = glm::vec3 {0.0f, 0.0f, 0.0f};
    // normalized
    // change upvec then change 0.000001, 0.000001 back to 0,0 because cross does not work, if the area is zero
    glm::vec3 direction_ = glm::vec3 {0.0f, 0.0f, -1.0f};
    glm::vec3 up_vector_ {0.0f, 1.0f, 0.0f};
    // of image
    int width_;
    int height_;
    // distance from cameraposition to cameraplane
    float distance_;
    float dof_strength_ = 0.0f;
    float focal_length_ = 1.0f;
    
};


#endif //RAYTRACER_CAMERA_HPP
