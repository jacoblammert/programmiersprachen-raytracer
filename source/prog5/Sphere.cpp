

#include <cmath>
#include "Sphere.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

/**
 * Sphere constructor with glm::vec3 for the center and a float radius
 * @param center pos in 3d space
 * @param radius as float, negative values will be made positive
 */
Sphere::Sphere(glm::vec3 center, float radius):center_(center),radius_(abs(radius)) {}

/**
 * @return area as float uses M_PI constant from <math.h> to compute the total area
 */
float const Sphere::area() {
    return 4 * M_PI * radius_;
}

/**
 * @return volume of the sphere as float uses M_PI constant from <math.h>
 */
float const Sphere::volume() {
    return (float)(4 * M_PI * pow(radius_,5))/3;
}


