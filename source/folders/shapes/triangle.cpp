#include "triangle.hpp"

/**
 * constructor for triangle with three vectors
 * sets and normalizes the normal for this triangle
 * @param a glm::vec3 number 1
 * @param b glm::vec3 number 2
 * @param c glm::vec3 number 3
 */
Triangle::Triangle (std::string const& name, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) :
        b_ {b},
        c_ {c}
{
    shape_type_ = TRIANGLE;
    name_ = name;
    position_ = a;
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    this->normal_ = glm::normalize(glm::cross(ab,ac));
}

/*
Triangle::Triangle(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, Color const& color) :
        a{a},
        b{b},
        c{c} {
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    this->normal = glm::normalize(glm::cross(ab,ac));
}
*/

/**
 * returns true, if the triangle was in front of the ray and the ray intersected the triangle
 * both sides of the triangle can be intersected (front and back) for refraction usefull
 * @param ray to be tested
 * @param hit_point position where the ray intersects the triangle first as reference
 * @param hit_normal The normal of the triangle at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the triangle is in front of the ray and has been intersected
 */
bool Triangle::get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {


    glm::vec3 p_vec = glm::cross(ray.direction_ , (c_ - position_));
    float det = glm::dot(p_vec, (b_ - position_));

    // ray and triangle are parallel if det is close to 0 -> no division by zero
    if (std::fabs(det) == 0.0f)
        return false;

    //glm::vec3 rayposition = ray.position;

    float u = glm::dot(p_vec, ray.position_ - position_) / det;

    if (u < 0 || u > 1)
        return false;

    p_vec = glm::cross((ray.position_ - position_), (b_ - position_));
    float v = glm::dot(p_vec, ray.direction_) / det;

    if (v < 0 || (u + v) > 1)
        return false;

    det = glm::dot(p_vec, c_ - position_) / det;

    if (0 < det && det < distance) {
        distance = det;
        p_vec = ray.direction_;
        p_vec *= det;
        hit_point = ray.position_ + p_vec;
        hit_normal = get_normal(ray.position_);
        return true;
    }
    return false;
}

/**
 * @param pos necessary to get the normal which is pointing "towards" the ray position
 * same reason for the plane
 * @return normal of the triangle
 */
glm::vec3 Triangle::get_normal(glm::vec3 const& pos) const {
    //if (glm::dot(glm::normalize(pos-this->a),normal) < 0) {
    //    return -normal;
    //} else{
        return normal_;
    //}
}

/**
 * @return a vector with the minimal values of x, y and z for the triangle (for a box around the triangle)
 */
glm::vec3 Triangle::get_min() const {
    glm::vec3 min = position_;

    for (int j = 0; j < 3; ++j) {
        if (min[j] > b_[j]) {
            min[j] = b_[j];
        }
    }

    for (int j = 0; j < 3; ++j) {
        if (min[j] > c_[j]) {
            min[j] = c_[j];
        }
    }
    return min;
}

/**
 * @return a vector with the maximal values of x, y and z for the triangle (for a box around the triangle)
 */
glm::vec3 Triangle::get_max() const {
    glm::vec3 max = position_;
    
    for (int j = 0; j < 3; ++j) {
        if (max[j] < b_[j]) {
            max[j] = b_[j];
        }
    }
    for (int j = 0; j < 3; ++j) {
        if (max[j] < c_[j]) {
            max[j] = c_[j];
        }
    }
    return max;
}

/**
 * @return all points added and divided by 3 to get a median point on the triangle
 */
glm::vec3 Triangle::get_median() const {

    glm::vec3 median = position_ + b_ + c_;
    median *= (1.0f / 3.0f);
    return median;
}

/**
* @return material of the sphere
*/
std::shared_ptr<Material> Triangle::get_material() {
    return material_;
}

/**
* @param material is given to sphere
*/
void Triangle::set_material(std::shared_ptr<Material> const& material) {
    this->material_ = material;
}

/**
 * outputs important information in the console
 */
void Triangle::print(std::fstream & file) const {
    std::cout << "Triangle" << std::endl;
    std::cout << "A: ";
    //a.print();
    std::cout << "B: ";
    //b.print();
    std::cout << "C: ";
    //c.print();
}

/**
 * Changes the position of the triangle with a given glm::vec3
 * @param position
 */
void Triangle::translate(glm::vec3 const& position) {
    position_ += position;
    b_ += position;
    c_ += position;
}

std::string Triangle::get_information() const {
    std::string information = std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " + std::to_string(position_[2]) + " "
            + std::to_string(b_[0]) + " " + std::to_string(b_[1]) + " " + std::to_string(b_[2]) + " "
            + std::to_string(c_[0]) + " " + std::to_string(c_[1]) + " " + std::to_string(c_[2]) + " "
            + material_->name_;
    return std::string();
}
