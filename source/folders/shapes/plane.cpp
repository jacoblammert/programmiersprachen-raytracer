//
// Created by Jacob Lammert on 17.08.2020.
//

#include <iostream>
#include <glm-0.9.5.3/glm/geometric.hpp>
#include "plane.h"

/**
 * generates a plane with a given position and normal vector
 * @param position for the new plane
 * @param normal of the pane (will be normalized automatically)
 */
plane::plane(glm::vec3 position, glm::vec3 normal):
pos{position},normal{normal}{
    glm::normalize(normal);
}
/*///TODO add later (with materials)
plane::Plane(Vector position, Vector normal, Material *material) :
        pos{position}, normal{normal} {
    this->normal.normalize();
    this->material = material;
}/**/

/**
 * returns true, if this plane has been intersected in front of the ray, no intersections with negative scalar for ray direction allowed
 * @param ray to be tested against the plane
 * @param HitPoint point of the ray plane intersection
 * @param HitNormal normal of the plane
 * @param distance as float (scalar for the normalized direction of the ray)
 * @return true, if intersected in front of the ray, false otherwise
 */
bool plane::getIntersectVec(ray ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {

    glm::vec3 raydirection = ray.direction;
    glm::vec3 rayposition = ray.position;

    float denom = glm::dot(normal,raydirection);
    glm::vec3 p0l0 = pos - rayposition;

    float t = glm::dot(p0l0,normal) / denom;

    if (t >= 0) {

        raydirection *= t;
        if (0 < t && t < distance) {
            distance = t;
            HitPoint = rayposition + raydirection;
            HitNormal = normal;
        }
        return true;
    }
    return false;
}

/**
 * @param pos - in this case useless parameter, better implementation was in script I believe (optional parameters)
 * - only here because of the sphere and maybe triangle
 * @return normal of the plane
 */
glm::vec3 plane::getNormal(glm::vec3 pos) const {
    return normal;
}

/**
 * @return minimum of the plane (useless, because we will leave planes in the first layer of our hierarchy anyways)
 */
glm::vec3 plane::getMin() const {
    return {-INFINITY,-INFINITY,-INFINITY}; // should give back a box of the desired size of the plane
}

/**
 * @return maximum of the plane (useless, because we will leave planes in the first layer of our hierarchy anyways)
 */
glm::vec3 plane::getMax() const {
    return {INFINITY,INFINITY,INFINITY}; // should give back a box of the desired size of the plane (cutting doesn't work)
}
/**
 * where is the median of an infinite plane? -> position is our best guess
 * @return position of the plane
 */
glm::vec3 plane::getMedian() const {
    return pos;
}

/**
 * should print "interesting" information about our plane (position, normal, material,...)
 */
void plane::print() const {
    std::cout << "Plane" << std::endl;
}

/*/
Material *plane::getMaterial() {
    return material;
}

void Plane::setMaterial(Material *material) {
    this->material = material;
}
/**/
/**
 * Changes the position of the Sphere with a given glm::vec3
 * @param position
 */
void plane::translate(glm::vec3 position) {
    pos += position;
}