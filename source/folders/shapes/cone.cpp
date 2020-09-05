#include "cone.hpp"


Cone::Cone (std::string const& name, const glm::vec3 &position, const glm::vec3 &axis, float width, float height) :
    width_ {width},
    height_ {height}

{
    shape_type_ = CONE;
    name_ = name;
    position_ = position;
    set_rotation_axis(axis);

    //double sin_angle = std::sin(angle);
    //double cos_angle = std::cos(angle);


    //rotation_matrix_ = glm::mat3x3{
    //        axis[0] * axis[0] * (1 - cos_angle) + cos_angle           ,axis[1] * axis[0] * (1 - cos_angle) - sin_angle * axis[2] ,axis[2] * axis[0] * (1 - cos_angle) + sin_angle * axis[1],
    //        axis[0] * axis[1] * (1 - cos_angle) + sin_angle * axis[2] ,axis[1] * axis[1] * (1 - cos_angle) + cos_angle           ,axis[2] * axis[1] * (1 - cos_angle) - sin_angle * axis[0] ,
    //        axis[0] * axis[2] * (1 - cos_angle) - sin_angle * axis[1] ,axis[1] * axis[2] * (1 - cos_angle) + sin_angle * axis[0] ,axis[2] * axis[2] * (1 - cos_angle) + cos_angle
    //};
}


bool Cone::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {

/// only for "normalized cone in origin with diameter 1 and height 1




    glm::vec3 up_vec = glm::normalize(glm::vec3{0, 0, 1}); // random vector used for rotation
    //glm::vec3 rotation_axis = glm::normalize(glm::cross(up_vec, axis_));
    //float angle_new = (float)acos(glm::dot(up_vec,axis_));// = glm::dot(rotation_axis_,up_vec);



    glm::vec3 ray_position = ray.position_ - position_;                                                               /// first translation
    ray_position = rotation_matrix_ * ray_position;//get_rotated_vec3(ray_position, rotation_axis, angle_new);                                         /// second rotation

    float inverse_width = 1.0f / width_;
    float inverse_height =  1.0f / height_;

    ray_position = ray_position * glm::vec3{ inverse_width ,  inverse_width,inverse_height};/// third scaling


    glm::vec3 ray_direction = ray.direction_;
    ray_direction = rotation_matrix_ * ray_direction;//get_rotated_vec3(ray_direction, rotation_axis, angle_new);                                        /// first rotation
    ray_direction = ray_direction * glm::vec3{ inverse_width ,  inverse_width,inverse_height};/// second scaling
    ray_direction = glm::normalize(ray_direction);                                                                    /// third normalize


    float m = 0.25f;
    glm::vec3 w = ray_position - up_vec;

    float w_theta = glm::dot(w, up_vec);


    float a = glm::dot(ray_direction, ray_direction) - m * (pow(glm::dot(ray_direction, up_vec), 2)) - pow(glm::dot(ray_direction, up_vec), 2);
    float b = 2 * (glm::dot(ray_direction, w) - m * glm::dot(ray_direction, up_vec) * w_theta - glm::dot(ray_direction, up_vec) * w_theta);
    float c = glm::dot(w, w) - m * (w_theta * w_theta) - (w_theta * w_theta);

    float det = b * b - (4 * a * c);

    float distance_1 = INFINITY;
    glm::vec3 hit_point_1;
    glm::vec3 hit_normal_1;


    if (det > 0) {
        det = sqrt(det);
        c =  (-b - det) / (2 * a);
        det =(-b + det) / (2 * a);

        if (c < 0 || det > 0 && det < c) {
            c = det;
        }
        if (c >= 0) {

            glm::vec3 cp = ray_position + ray_direction * c;

            if (!(cp[2] < 0 || cp[2] > 1)) {

                hit_point_1 = cp;

                hit_point_1 = get_scaled_vec3(hit_point_1, width_,  width_,height_);   /// first scaling

                hit_point_1 = rotation_matrix_inverse * hit_point_1;

                hit_point_1 = hit_point_1 + position_;                                 /// third translation

                c = glm::length(hit_point_1 - ray.position_);

                distance_1 = (float) c;

                hit_normal_1 = get_normal(hit_point_1);
            }
        }
    }

    float distance_2 = glm::dot(position_ - ray.position_, axis_) / glm::dot(ray.direction_, axis_);

    /// either the cone or the bottom plate has been hit


    float length = glm::length(ray.position_ - position_ + ray.direction_ * distance_2);


        if (0 < distance_2 && distance_2 < distance && distance_2 < distance_1 && length < (width_ / 2)) {

            distance = distance_2;
            hit_point = ray.position_ + ray.direction_ * distance_2;
            hit_normal = -axis_;
            return true;
        }
        if (0 < distance_1 && distance_1 < distance) {
            distance = distance_1;
            hit_point = hit_point_1;
            hit_normal = hit_normal_1;
            return true;
        }
    return false;
}

glm::vec3 Cone::get_normal(const glm::vec3 &pos) const {

    glm::vec3 v = pos - position_;
    v.y = 0;
    v = glm::normalize(v);

    glm::vec3 n;
    n.x = (v.x * height_/(width_/2));
    n.z = v.z * height_/(width_/2);
    n.y = (width_/2)/height_;

    return glm::normalize(n);
}

glm::vec3 Cone::get_min() const {
    return position_ - (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
}

glm::vec3 Cone::get_max() const {
    return position_ + (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
}

glm::vec3 Cone::get_median() const {
    return position_;
}

std::shared_ptr<Material> Cone::get_material() {
    return material_;
}

void Cone::set_material(const std::shared_ptr<Material> &material) {
    this->material_ = material;
}

void Cone::print() const {

}

void Cone::translate(const glm::vec3 &position) {
    position_ += position;
}

