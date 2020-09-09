
#include "cylinder.hpp"

#include <cmath>

Cylinder::Cylinder(std::string const &name, const glm::vec3 &position, const glm::vec3 &axis, float width, float height)
        :
        width_{width},
        height_{height} {
    shape_type_ = CYLINDER;
    name_ = name;
    position_ = position;
    set_rotation_axis(axis);
}

bool Cylinder::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {


    float distance_0 = glm::dot(position_ - ray.position, axis_) / glm::dot(ray.direction, axis_);
    float length_0 = glm::length(ray.position - position_ + ray.direction * distance_0);

    bool hit_0 = length_0 < width_ / 2 && distance_0 < distance && 0 < distance_0;


    float distance_1 = glm::dot(position_ - ray.position + axis_ * height_, axis_) / glm::dot(ray.direction, axis_);
    float length_1 = glm::length(ray.position - position_ - axis_ * height_ + ray.direction * distance_1);

    bool hit_1 = length_1 < width_ / 2 && distance_1 < distance && 0 < distance_1;


    glm::vec3 ray_position = ray.position;
    glm::vec3 ray_direction = ray.direction;


    /// Translation
    ray_position = ray_position - position_;


    /// Rotation
    glm::vec3 up_vec = glm::vec3{0, 0, 1};
    ray_position = rotation_matrix_ * ray_position;

    ray_direction = rotation_matrix_ * ray_direction;


    /// Scalierung
    float inverse_width = 1.0f / width_;
    float inverse_height = 1.0f / height_;

    ray_position = get_scaled_vec3(ray_position, inverse_width, inverse_width, inverse_height);

    ray_direction = get_scaled_vec3(ray_direction, inverse_width, inverse_width, inverse_height);
    ray_direction = glm::normalize(ray_direction);




    double a = ray_direction[0] * ray_direction[0] + ray_direction[1] * ray_direction[1];
    double b = ray_direction[0] * ray_position[0] + ray_direction[1] * ray_position[1];
    double c = ray_position[0] * ray_position[0] + ray_position[1] * ray_position[1] - (1.0f / 2) * (1.0f / 2);

    double delta = b * b - a * c;



    // nearest intersection
    bool hit_2 = false;
    float distance_2 = (float) ((-b - sqrt(delta)) / a);
    b = (float) ((-b + sqrt(delta)) / a);

    distance_2 = 0 < b && (b < distance_2 || distance_2 < 0) ? (float)b : distance_2;

    // c is now epsilon  Variables are reused to reuse the space and not create to many variables
    c = 0.00000001;

    // t<0 means the intersection is behind the ray origin
    // which we don't want
    if (distance_2 >= c) {


        a = ray_position[2] + distance_2 * ray_direction[2];

        // check if we intersect one of the bases
        if (!(a > 1 + c || a < -c)) {
            hit_2 = true;
        }
    }

    glm::vec3 hit_point_1; // we need these variables to not contaminate the result
    glm::vec3 hit_normal_1; // we need these variables to not contaminate the result

    if (!hit_0) {
        distance_0 = INFINITY;
    }
    if (!hit_1) {
        distance_1 = INFINITY;
    }
    if (!hit_2) {
        distance_2 = INFINITY;
    } else {

        hit_point_1 = ray_position + ray_direction * distance_2;
        hit_normal_1 = get_normal(hit_point_1);

        hit_point_1 = get_scaled_vec3(hit_point_1, width_, width_, height_);/// first scaling
        hit_point_1 = rotation_matrix_inverse * hit_point_1;                /// second rotation
        hit_point_1 = hit_point_1 + position_;                              /// third translation


        distance_2 = glm::length(ray.position - hit_point_1);
    }


    if (hit_0 || hit_1 || hit_2) {

        if (hit_0 && distance_0 < distance_1 && distance_0 < distance_2 && distance_0 < distance) {
            hit_point = ray.position + ray.direction * distance_0;
            distance = distance_0;
            hit_normal = glm::vec3{0, 0, -1};
        } else if (hit_1 && distance_1 < distance_0 && distance_1 < distance_2 && distance_1 < distance) {
            hit_point = ray.position + ray.direction * distance_1;
            distance = distance_1;
            hit_normal = glm::vec3{0, 0, 1};
        } else if (distance_2 < distance) {
            distance = distance_2;
            hit_point = hit_point_1;
            hit_normal = hit_normal_1;
        } else {
            return false;
        }


        hit_normal = get_scaled_vec3(hit_normal, width_, width_, height_);   /// first scaling
        hit_normal = rotation_matrix_inverse * hit_normal;/// second rotation
        hit_normal = glm::normalize(hit_normal);
        return true;
    }
    return false;
}

glm::vec3 Cylinder::get_normal(const glm::vec3 &pos) const {
    return glm::normalize(pos - glm::vec3{0, 0, pos[2]});
}

glm::vec3 Cylinder::get_min() const {
    return position_ - (glm::vec3{1, 1, 1} * std::sqrt(height_ * height_ + width_ * width_));
}

glm::vec3 Cylinder::get_max() const {
    return position_ + (glm::vec3{1, 1, 1} * std::sqrt(height_ * height_ + width_ * width_));
}

glm::vec3 Cylinder::get_median() const {
    return position_;
}

std::shared_ptr<Material> Cylinder::get_material() {
    return material_;
}

void Cylinder::set_material(const std::shared_ptr<Material> &material) {
    material_ = material;
}

void Cylinder::print(std::fstream &file) const {

}

void Cylinder::translate(const glm::vec3 &position) {
    position_ += position;
}

std::string Cylinder::get_information() const {
    std::string information = name_ + " " + std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " +
                              std::to_string(position_[2]) + " "
                              + std::to_string(axis_[0]) + " " + std::to_string(axis_[1]) + " " +
                              std::to_string(axis_[2]) + " "
                              + std::to_string(width_) + " " + std::to_string(height_) + " " + material_->name;
    return information;
}
