


#include "Sphere.h"
#include <cmath>

#define _USE_MATH_DEFINES

#include <math.h>
#include <glm-0.9.5.3/glm/glm.hpp>


/***
 * Constructor using the Constructor of the base class
 * @param center center position in 3d space
 * @param radius radius as float, negative values will be made positive
 * @param name as String, using Shape constructor
 * @param color as Color, using Shape constructor
 */
Sphere::Sphere(glm::vec3 center, float radius, std::string name, Color color) :
        center_(center), radius_(abs(radius)), Shape(name, color) {
}

/**
 * @return area as float uses M_PI constant from <math.h> to compute the total area
 */
float Sphere::area() const {
    return 4 * M_PI * radius_;
}

/**
 * @return volume of the sphere as float uses M_PI constant from <math.h>
 */
float Sphere::volume() const {
    return (float) (4 * M_PI * pow(radius_, 3)) / 3;
}

std::ostream &Sphere::print(std::ostream &os) const {
    Shape::print(os);

    os << "Center: " << center_[0] << " , " << center_[1] << " , " << center_[2] << "\nRadius: " << radius_ << "\n";
    os << "Area: " << area() << "\nVolume: " << volume() << "\n\n";
    return os;
}

/**
 * Intersection function returns a bool if a ray is hit in front of the ray in the raydirection.
 * If a Sphere is behind the ray, it returns false even, if it may have intersected the Sphere
 * @param ray with origin and direction
 * @return bool + info inside a HitPoint object, if a Sphere has been hit
 */
HitPoint Sphere::intersect(Ray ray) const {

    glm::vec3 raydirection = glm::normalize(ray.direction);

    float t = glm::dot(ray.direction, center_ - ray.origin);
    raydirection *= t;

    glm::vec3 vec3 = center_ - ray.origin - raydirection;

    float y = glm::length(vec3);

    if (y < radius_) {

        float x = sqrt(radius_ * radius_ - y * y);
        float t1 = t - x; // close intersection point
        float t2 = t + x; // far intersection point

        raydirection = ray.direction;

        float dist = 0;

        if (0 < t1) {
            raydirection * t1;
            dist = t1;
        } else if (t1 < 0 && 0 < t2) {
            raydirection * t2;
            dist = t2;
        }
        return {true, dist, name_, color_, ray.origin + raydirection, ray.direction};
    }
    return {false};
}




