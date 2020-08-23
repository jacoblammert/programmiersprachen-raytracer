//
// Created by Jacob Lammert on 17.08.2020.
//

#include "sphere.hpp"

/**
 * Sphere with a given Position and radius
 * @param position as glm::vec3 for x, y, z
 * @param radius value of input, if radius = -10, radius will be made positive
 */
Sphere::Sphere(const glm::vec3 &position, float radius) :
        pos{position},
        radius{abs(radius)}
{}

/**
 * returns true, if the sphere was in front of the ray and the ray intersected the sphere
 * @param ray to be tested
 * @param HitPoint position where the ray intersects the sphere first (for two intersections the closest point will be returned) as reference
 * @param HitNormal The normal of the sphere at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the sphere is in front of the ray and has been intersected
 */
bool Sphere::get_intersect_vec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {

    float t = glm::dot(pos - ray.position,ray.direction);
    float x = glm::length(pos - ray.position - (ray.direction * t));

    if (x < radius) {

        x = sqrt(radius * radius - x * x);

        // "improved" code by reusing variables & removing some for simplicity/ speed

        if (0 < (t - x) && (t - x) < distance) {
            distance = (t - x);
            HitPoint = ray.position + (ray.direction * (t - x));
            HitNormal = get_normal(HitPoint);
            return true;
        }

        if ((t - x) < 0 && 0 < (t + x) && (t + x) < distance) {
            distance = (t + x);
            HitPoint = ray.position + (ray.direction * (t + x));
            HitNormal = get_normal(HitPoint);
            return true;
        }
    }
    return false;
}

/**
 * returns the normal for a given position
 * @param position position (Hitposition)
 * @return normal (Vector from the Center to the input vector)
 */
glm::vec3 Sphere::get_normal(glm::vec3 const& position) const {
    glm::vec3 normal = position - this->pos;
    //if (glm::length(normal) > this->radius){
    //    return glm::normalize(normal);
    //} else{
        return glm::normalize(normal);
    //}
}

/**
 * @return a vector with the minimal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::get_min() const {
    glm::vec3 rad = glm::vec3(radius, radius, radius);
    return pos - rad;
}


/**
 * @return a vector with the maximal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::get_max() const {
    glm::vec3 rad = glm::vec3(radius, radius, radius);
    return pos + rad;
}

/**
 * @return position of the sphere
 */
glm::vec3 Sphere::get_median() const {
    return pos;
}

/**
 * //TODO implement a good print function
 * outputs important information in the console
 */
void Sphere::print() const {
    std::cout << "Sphere" << std::endl;
    //print();
}

/*/ //TODO add material later
Material* Sphere::get_material() {
    return material;
}

void Sphere::set_material(Material* material) {
    this->material = material;
}/**/

/**
 * Changes the position of the sphere with a given glm::vec3
 * @param position
 */
void Sphere::translate(glm::vec3 const& position) {
    pos += position;
}
