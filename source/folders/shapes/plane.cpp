//
// Created by Jacob Lammert on 17.08.2020.
//

#include "plane.hpp"

/**
 * generates a plane with a given position and normal vector
 * @param position for the new plane
 * @param normal of the pane (will be normalized automatically)
 */
Plane::Plane(glm::vec3 const &position, glm::vec3 const &normal) :
        pos{position}, normal{glm::normalize(normal)} {
}
/*///TODO add later (with materials)
plane::Plane(Vector const& position, Vector const& normal, Material *material) :
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
bool Plane::get_intersect_vec(Ray const &ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {
    float t = glm::dot(pos - ray.position, normal) / glm::dot(ray.direction,normal);

    if (0 <= t && t < distance) {
        distance = t;
        HitPoint = ray.position + ray.direction * t;
        HitNormal = get_normal(ray.position);
        return true;
    }
    return false;
}

/**
 * @param pos of the camera very important in order to get correct shadows from both sides of the plane
 * @return normal of the plane always "pointing" towards the camera
 */
glm::vec3 Plane::get_normal(glm::vec3 const &pos) const {
    //if (glm::dot(glm::normalize(pos-this->pos),normal) < 0) {
    //    return -normal;
    //} else{
        return normal;
    //}
}

/**
 * @return minimum of the plane (useless, because we will leave planes in the first layer of our hierarchy anyways)
 */
glm::vec3 Plane::get_min() const {
    return {-INFINITY, -INFINITY, -INFINITY}; // should give back a box of the desired size of the plane
}

/**
 * @return maximum of the plane (useless, because we will leave planes in the first layer of our hierarchy anyways)
 */
glm::vec3 Plane::get_max() const {
    return {INFINITY, INFINITY,
            INFINITY}; // should give back a box of the desired size of the plane (cutting doesn't work)
}

/**
 * where is the median of an infinite plane? -> position is our best guess
 * @return position of the plane
 */
glm::vec3 Plane::get_median() const {
    return pos;
}

/**
 * should print "interesting" information about our plane (position, normal, material,...)
 */
void Plane::print() const {
    std::cout << "Plane" << std::endl;
    //print();
}

/*/
Material *Plane::getMaterial() {
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
void Plane::translate(glm::vec3 const &position) {
    pos += position;
}
