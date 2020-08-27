#include "cone.hpp"
#include "plane.hpp"

bool Cone::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {

/// only for "normalized cone in origin with diameter 1 and height 1

    glm::vec3 up_vec = glm::vec3{0, 0, 1}; // random vector used for rotation

    float angle_new = (float) (1-glm::dot(rotation_axis_, up_vec)) * 3.14159265359f/2.0f;



    //std::cout<< "angle in radians: " << angle_new<<std::endl;

    glm::vec3 rotation_axis = glm::normalize(glm::cross(up_vec, rotation_axis_));


    glm::vec3 ray_position = ray.position_ - position_;                                                               /// first translation
    ray_position = get_rotated_vec3(ray_position, rotation_axis, angle_new);                                         /// second rotation
    ray_position = get_scaled_vec3(ray_position,  1.0f / width_,  1.0f / width_, 1.0f / height_);/// third scaling


    glm::vec3 ray_direction = ray.direction_;
    ray_direction = get_rotated_vec3(ray_direction, rotation_axis, angle_new);                                        /// first rotation
    ray_direction = get_scaled_vec3(ray_direction, 1.0f / width_,  1.0f / width_, 1.0f / height_);/// second scaling
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

                hit_point_1 = get_scaled_vec3(hit_point_1, width_, width_, height_);   /// first scaling

                hit_point_1 = get_rotated_vec3(hit_point_1, rotation_axis, -angle_new);/// second rotation

                hit_point_1 = hit_point_1 + position_;                                 /// third translation

                c = glm::length(hit_point_1 - ray.position_);

                distance_1 = (float) c;

                //hit_normal_1 = glm::normalize(cp * (glm::dot(glm::vec3 {0,0,1}, cp) / glm::dot(cp, cp)) - glm::vec3 {0,0,1});

                //hit_normal_1 = cp * (cp[2] / glm::dot(cp, cp)) - glm::vec3{0, 0, 1};

                cp[2] = 0;
                cp = glm::normalize(cp);
                cp[2] = 2;

                hit_normal_1 = glm::normalize(cp);//glm::vec3 {0.5 * sin(cp[0]),0.5 *  cos(cp[0]),1});

                hit_normal_1 = get_scaled_vec3(hit_normal_1,width_, width_, height_);    /// first scaling

                hit_normal_1 = get_rotated_vec3(hit_normal_1, rotation_axis, -angle_new);/// second rotation

                hit_normal_1 = glm::normalize(hit_normal_1);                             /// third normalize
            }
        }
    }

    float distance_2 = glm::dot(position_ - ray.position_, rotation_axis_) / glm::dot(ray.direction_, rotation_axis_);

    /// either the cone or the bottom plate has been hit


    float length = glm::length(ray.position_ - position_ + ray.direction_ * distance_2);


        if (0 < distance_2 && distance_2 < distance && distance_2 < distance_1 && length < (width_ / 2)) {

            distance = distance_2;
            hit_point = ray.position_ + ray.direction_ * distance_2;
            hit_normal = -rotation_axis_;
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
    return glm::normalize(pos - position_);
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

Cone::Cone(const glm::vec3 &position, const glm::vec3 &axis, float width, float height) :
        width_{width}, height_{height} {
    position_ = position;
    rotation_axis_ = glm::normalize(axis);


    //double sin_angle = std::sin(angle);
    //double cos_angle = std::cos(angle);


    //rotation_matrix_ = glm::mat3x3{
    //        axis[0] * axis[0] * (1 - cos_angle) + cos_angle           ,axis[1] * axis[0] * (1 - cos_angle) - sin_angle * axis[2] ,axis[2] * axis[0] * (1 - cos_angle) + sin_angle * axis[1],
    //        axis[0] * axis[1] * (1 - cos_angle) + sin_angle * axis[2] ,axis[1] * axis[1] * (1 - cos_angle) + cos_angle           ,axis[2] * axis[1] * (1 - cos_angle) - sin_angle * axis[0] ,
    //        axis[0] * axis[2] * (1 - cos_angle) - sin_angle * axis[1] ,axis[1] * axis[2] * (1 - cos_angle) + sin_angle * axis[0] ,axis[2] * axis[2] * (1 - cos_angle) + cos_angle
    //};
}
