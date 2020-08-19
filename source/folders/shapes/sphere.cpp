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
bool Sphere::getIntersectVec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {

    glm::vec3 raydirection = ray.direction;


    float t = glm::dot((pos - ray.position), raydirection); // TODO check if right function was imported for dot


    raydirection *= t;

    float y = ((pos - ray.position) - raydirection).length();

    if (y < radius) {

        float x = std::sqrt(radius * radius - y * y);
        float t1 = t - x; // close intersection point
        float t2 = t + x; // far intersection point

        raydirection = glm::normalize(ray.direction);


        if (0 < t1 && t1 < distance) {
            raydirection * t1;
            distance = t1;
            HitPoint = ray.position + raydirection;
            HitNormal = getNormal(HitPoint);
            return true;
        }
        if (t1 < 0 && 0 < t2 && t2 < distance) {
            raydirection * t2;
            distance = t2;
            HitPoint = ray.position + raydirection;
            HitNormal = getNormal(HitPoint);
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
glm::vec3 Sphere::getNormal(glm::vec3 const& position) const {
    glm::vec3 normal = position - this->pos;
    glm::normalize(normal);
    return normal;
}

/**
 * @return a vector with the minimal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::getMin() const {
    glm::vec3 rad = glm::vec3(radius, radius, radius);
    return pos - rad;
}


/**
 * @return a vector with the maximal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::getMax() const {
    glm::vec3 rad = glm::vec3(radius, radius, radius);
    return pos + rad;
}

/**
 * @return position of the sphere
 */
glm::vec3 Sphere::getMedian() const {
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
Material* Sphere::getMaterial() {
    return material;
}

void Sphere::setMaterial(Material* material) {
    this->material = material;
}/**/

/**
 * Changes the position of the sphere with a given glm::vec3
 * @param position
 */
void Sphere::translate(glm::vec3 const& position) {
    pos += position;
}
