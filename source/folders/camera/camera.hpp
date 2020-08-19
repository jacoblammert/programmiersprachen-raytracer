//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#define MATH_PI 3.14159265359

#include <glm-0.9.5.3/glm/geometric.hpp>
#include <cmath>
#include <iostream>
#include "../camera/ray.hpp"
//#include "../math/Vector.h"


class Camera {
public:
    Camera(glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance);
    Ray generateRay(int x, int y) const;
    void lookAt(glm::vec3 const& pos);
    void print();
    //void translate(glm::vec3 const& position);

private:
    glm::vec3 position;  /// could be put together with direction to generate a ray with pos/ direction
    glm::vec3 direction; // normalized
    int width;  // of image
    int height; // of image
    float distance; // distance from cameraposition to cameraplane
    glm::vec3 upVector{0,0,1};
};


#endif //RAYTRACER_CAMERA_H
