

#include "Box.h"
#include <cmath>

/**
 * Box constructor
 * the diagonal will be max_ - min_
 * @param min as glm::vec3
 * @param max as glm::vec3
 */
Box::Box(glm::vec3 min, glm::vec3 max):max_(max),min_(min)
{}

/**
 * @return total area of a box as float
 */
float const Box::area() {
    glm::vec3 diagonal = max_ - min_;
    float x = diagonal[0];
    float y = diagonal[1];
    float z = diagonal[2];
    return 2 * (fabsf(x * y) + fabsf(x * z) + fabsf(y * z));
}

/**
 * @return total volume as a box as float
 */
float const Box::volume() {
    glm::vec3 diagonal = max_ - min_;
    float x = diagonal[0];
    float y = diagonal[1];
    float z = diagonal[2];
    return fabsf(x * y * z);
}


