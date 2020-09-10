#include "cone.hpp"

/**
* Initializer with a vector of position and axis and width and height
 @param name name of cone object
 @param position of cone
 @param axis for rotation
 @param width
 @param height
*/
Cone::Cone(std::string const &name, const glm::vec3 &position, const glm::vec3 &axis, float width, float height) :
        width_{width},
        height_{height}
{
    shape_type_ = CONE;
    name_ = name;
    position_ = position;
    set_rotation_axis(axis);
}

/**
* To check if the cone object has been hit
 * @param ray to be tested
 * @param hit_point position where the ray intersects the cone first (for two intersections the closest point will be returned) as reference
 * @param hit_normal The normal of the cone at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 @return true, of the cone is in front of the ray and has been intersected
*/
bool Cone::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {

    // only for "normalized cone in origin with diameter 1 and height 1

    glm::vec3 up_vec = glm::normalize(glm::vec3{0, 0, 1});
    // first translation
    glm::vec3 ray_position = ray.position - position_;
    // second rotation
    ray_position = rotation_matrix_ * ray_position;                                         

    float inverse_width = 1.0f / width_;
    float inverse_height = 1.0f / height_;

    // third scaling
    ray_position = ray_position * glm::vec3{inverse_width, inverse_width, inverse_height};
    glm::vec3 ray_direction = ray.direction;
    // first rotation
    ray_direction = rotation_matrix_ * ray_direction;
    // second scaling
    ray_direction = ray_direction * glm::vec3{inverse_width, inverse_width, inverse_height};
    // third normalize
    ray_direction = glm::normalize(ray_direction);

    float m = 0.25f;
    glm::vec3 w = ray_position - up_vec;

    float w_theta = glm::dot(w, up_vec);


    float a = (float) (glm::dot(ray_direction, ray_direction) - m * (pow(glm::dot(ray_direction, up_vec), 2)) - pow(glm::dot(ray_direction, up_vec), 2));
    float b = 2 * (glm::dot(ray_direction, w) - m * glm::dot(ray_direction, up_vec) * w_theta -
                   glm::dot(ray_direction, up_vec) * w_theta);
    w_theta = glm::dot(w, w) - m * (w_theta * w_theta) - (w_theta * w_theta);

    m = b * b - (4 * a * w_theta);

    float distance_1 = INFINITY;
    glm::vec3 hit_point_1;
    glm::vec3 hit_normal_1;


    if (m > 0) {
        m = (float) sqrt(m);
        w_theta = (-b - m) / (2 * a);
        m = (-b + m) / (2 * a);

        if (w_theta < 0 || (m > 0 && m < w_theta)) {
            w_theta = m;
        }
        if (w_theta >= 0) {

            glm::vec3 cp = ray_position + ray_direction * w_theta;

            if (!(cp[2] < 0 || cp[2] > 1)) {

                hit_point_1 = cp;

                // first scaling
                hit_point_1 = get_scaled_vec3(hit_point_1, width_, width_, height_);

                hit_point_1 = rotation_matrix_inverse * hit_point_1;
                
                // third translation
                hit_point_1 = hit_point_1 + position_;

                w_theta = glm::length(hit_point_1 - ray.position);

                distance_1 = (float) w_theta;

                hit_normal_1 = get_normal(cp);
            }
        }
    }

    float distance_2 = glm::dot(position_ - ray.position, axis_) / glm::dot(ray.direction, axis_);

    // either the cone or the bottom plate has been hit

    float length = glm::length(ray.position - position_ + ray.direction * distance_2);


    if (0 < distance_2 && distance_2 < distance && distance_2 < distance_1 && length < (width_ / 2)) {

        distance = distance_2;
        hit_point = ray.position + ray.direction * distance_2;
        hit_normal = -axis_;
        return true;
    }
    if (0 < distance_1 && distance_1 < distance) {
        distance = distance_1;
        hit_point = hit_point_1;

        // first scaling
        hit_normal = get_scaled_vec3(hit_normal_1, width_, width_, height_);
        // second rotation
        hit_normal = rotation_matrix_inverse * hit_normal;
        hit_normal = glm::normalize(hit_normal);
        return true;
    }
    return false;
}

/**
* @param pos necessary to get the normal which is pointing "towards" the ray position
* same reason for the plane
* @return normal of the cone
*/
glm::vec3 Cone::get_normal(const glm::vec3 &pos) const {
    glm::vec3 v = glm::normalize(glm::vec3{pos[0],pos[1],0});
    v.x *= 2;
    v.y *= 2;
    v.z = 0.5f;
    return glm::normalize(v);
}

/**
* @return a vector with the minimal values of x, y and z for the cone
*/
glm::vec3 Cone::get_min() const {
    return position_ - (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
}

/**
* @return a vector with the maximal values of x, y and z for the cone
*/
glm::vec3 Cone::get_max() const {
    return position_ + (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
}

/**
* @return position of the cone
*/
glm::vec3 Cone::get_median() const {
    return position_;
}

/**
* @return material of the cone
*/
std::shared_ptr<Material> Cone::get_material() {
    return material_;
}

/**
* @param material is given to cone
*/
void Cone::set_material(const std::shared_ptr<Material> &material) {
    this->material_ = material;
}

/**
* Changes the position of the cone with a given glm::vec3
* @param position
*/
void Cone::translate(const glm::vec3 &position) {
    position_ += position;
}

/**
*Gives information of the cone
* @returns string with cone name and data
*/
std::string Cone::get_information() const {
    std::string information = name_ + " " + std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " +
                              std::to_string(position_[2]) + " "
                              + std::to_string(axis_[0]) + " " + std::to_string(axis_[1]) + " " +
                              std::to_string(axis_[2]) + " "
                              + std::to_string(width_) + " " + std::to_string(height_) + " " + material_->name;
    return information;
}

