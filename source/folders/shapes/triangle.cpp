//
// Created by Jacob Lammert on 17.08.2020.
//

#include "triangle.hpp"

/**
 * constructor for triangle with three vectors
 * sets and normalizes the normal for this triangle
 * @param a glm::vec3 number 1
 * @param b glm::vec3 number 2
 * @param c glm::vec3 number 3
 */
Triangle::Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) :
        a{a},
        b{b},
        c{c} {
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    this->normal = glm::cross(ab,ac);
    glm::normalize(this->normal);
}

/*/
Triangle::Triangle(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, Color const& color) :
        a{a},
        b{b},
        c{c} {
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    this->normal = glm::cross(ab,ac);
    glm::normalize(this->normal);
}/**/

/**
 * returns true, if the triangle was in front of the ray and the ray intersected the triangle
 * both sides of the triangle can be intersected (front and back) for refraction usefull
 * @param ray to be tested
 * @param HitPoint position where the ray intersects the triangle first as reference
 * @param HitNormal The normal of the triangle at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the triangle is in front of the ray and has been intersected
 */
bool Triangle::getIntersectVec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {


    glm::vec3 pvec = glm::cross(ray.direction , (c - a));
    float det = glm::dot(pvec, (b - a));

    // ray and triangle are parallel if det is close to 0 -> no division by zero
    if (std::fabs(det) == 0.0f)
        return false;

    glm::vec3 rayposition = ray.position;

    float u = glm::dot(pvec, rayposition - a) / det;

    if (u < 0 || u > 1)
        return false;

    pvec = glm::cross((rayposition - a) , (b - a));
    float v = glm::dot(pvec, ray.direction) / det;

    if (v < 0 || (u + v) > 1)
        return false;

    det = glm::dot(pvec, c - a) / det;

    if (0 < det && det < distance) {
        distance = det;
        HitNormal = this->normal;
        pvec = ray.direction;
        pvec *= det;
        HitPoint = rayposition + pvec;
    }
    return true;
}

/**
 * @param pos in this case not necessary, only for box, cone/ cylinder (both not yet implented) and sphere
 * @return normal of the triangle
 */
glm::vec3 Triangle::getNormal(glm::vec3 const& pos) const {
    return this->normal;
}

/**
 * @return a vector with the minimal values of x, y and z for the triangle (for a box around the triangle)
 */
glm::vec3 Triangle::getMin() const {
    glm::vec3 min = a;

    for (int j = 0; j < 3; ++j) {
        if (min[j] > b[j]) {
            min[j] = b[j];
        }
    }

    for (int j = 0; j < 3; ++j) {
        if (min[j] > c[j]) {
            min[j] = c[j];
        }
    }
    return min;
}

/**
 * @return a vector with the maximal values of x, y and z for the triangle (for a box around the triangle)
 */
glm::vec3 Triangle::getMax() const {
    glm::vec3 max = a;
    for (int j = 0; j < 3; ++j) {
        if (max[j] < b[j]) {
            max[j] = b[j];
        }
    }
    for (int j = 0; j < 3; ++j) {
        if (max[j] < c[j]) {
            max[j] = c[j];
        }
    }
    return max;
}

/**
 * @return all points added and divided by 3 to get a median point on the triangle
 */
glm::vec3 Triangle::getMedian() const {

    glm::vec3 median = a + b + c;
    median *= (1.0f / 3.0f);
    return median;
}

/**
 * outputs important information in the console
 */
void Triangle::print() const {
    std::cout << "Triangle" << std::endl;
    std::cout << "A: ";
    //a.print();
    std::cout << "B: ";
    //b.print();
    std::cout << "C: ";
    //c.print();
}

/*/
Material *Triangle::getMaterial() const{
    return this->material;
}

void Triangle::setMaterial(Material *material) {
    this->material = material;
}/**/

/**
 * Changes the position of the triangle with a given glm::vec3
 * @param position
 */
void Triangle::translate(glm::vec3 const& position) {
    a += position;
    b += position;
    c += position;
}
