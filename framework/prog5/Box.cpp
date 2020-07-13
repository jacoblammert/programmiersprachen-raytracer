

#include "Box.h"
#include <cmath>

/**
 * Box constructor
 * the diagonal will be max_ - min_
 * @param min as glm::vec3
 * @param max as glm::vec3
 * @param name as string
 * @param color r g b
 */
Box::Box(glm::vec3 min, glm::vec3 max, std::string name, Color color):
max_(max),min_(min),Shape(name,color)
{}


/**
 * @return total area of a box as float
 */
float Box::area() const {
    glm::vec3 diagonal = max_ - min_;
    float x = fabsf(diagonal[0]);
    float y = fabsf(diagonal[1]);
    float z = fabsf(diagonal[2]);
    return 2 * (x * y + x * z + y * z);
}

/**
 * @return total volume as a box as float
 */
float Box::volume() const {
    glm::vec3 diagonal = max_ - min_;
    float x = fabsf(diagonal[0]);
    float y = fabsf(diagonal[1]);
    float z = fabsf(diagonal[2]);
    return x * y * z;
}




