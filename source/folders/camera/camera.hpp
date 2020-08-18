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
    void generateRay(int x, int y, Ray const& ray) const;
    void lookAt(glm::vec3 const& pos);
    void print();
    //void translate(glm::vec3 const& position);

private:
    glm::vec3 position;
    glm::vec3 direction;
    int width;
    int height;
    glm::vec3 distance;
    glm::vec3 upVector;
};


#endif //RAYTRACER_CAMERA_H
