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
    this->axis_ = glm::normalize(glm::cross(ab,ac));
}



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


    glm::vec3 p_vec = glm::cross(ray.direction , (c_ - position_));
    float det = glm::dot(p_vec, (b_ - position_));

    // ray and triangle are parallel if det is close to 0 -> no division by zero
    if (std::fabs(det) == 0.0f)
        return false;

    float u = glm::dot(p_vec, ray.position - position_) / det;

    if (u < 0 || u > 1)
        return false;

    p_vec = glm::cross((ray.position - position_), (b_ - position_));
    float v = glm::dot(p_vec, ray.direction) / det;

    if (v < 0 || (u + v) > 1)
        return false;

    det = glm::dot(p_vec, c_ - position_) / det;

    if (0 < det && det < distance) {
        distance = det;
        p_vec = ray.direction;
        p_vec *= det;
        hit_point = ray.position + p_vec;
        hit_normal = get_normal(ray.position);
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
    return axis_;
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
    median /= 3.0f;
    return median;
}

/**
* @return material of the triangle
*/
std::shared_ptr<Material> Triangle::get_material() {
    return material_;
}

/**
* @param material is given to triangle
*/
void Triangle::set_material(std::shared_ptr<Material> const& material) {
    this->material_ = material;
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

/**
*Gives information of the triangle
* @returns string with name and data
*/
std::string Triangle::get_information() const {
    std::string information = name_+ " "
            + std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " + std::to_string(position_[2]) + " "
            + std::to_string(b_[0]) + " " + std::to_string(b_[1]) + " " + std::to_string(b_[2]) + " "
            + std::to_string(c_[0]) + " " + std::to_string(c_[1]) + " " + std::to_string(c_[2]) + " "
            + material_->name;
    return information;
}

/**
 * This function lets you set the normal of the triangle.
 * The position_ stays the same, therefore a and b must change and rotate around the position
 * in a way that the new rotated vector form a triangle with the axis_ standing perpendicular on its plane
 *
 * @param axis
 */
void Triangle::set_rotation_axis(const glm::vec3 &axis) {

    Shape::set_rotation_axis(axis);

    glm::vec3 ab = b_ - position_;
    glm::vec3 ac = c_ - position_;

    float length_b = glm::length(ab);
    float length_c = glm::length(ac);

    ab = glm::normalize(ab);
    ac = glm::normalize(ac);

    glm::vec3 up_vec = glm::normalize(glm::vec3{0, 0, 1}); // vector used for rotation
    glm::vec3 rotation_axis = glm::normalize(glm::cross(up_vec, axis_));
    float angle = (float)acos(glm::dot(up_vec,axis_));


    ab = glm::normalize(get_rotated_vec3(ab,rotation_axis,angle));
    ac = glm::normalize(get_rotated_vec3(ac,rotation_axis,angle));

    ab *= length_b;
    ac *= length_c;

    b_ = ab + position_;
    c_ = ac + position_;

}
