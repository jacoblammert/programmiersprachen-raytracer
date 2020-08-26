#include <cmath>
#include <glm-0.9.5.3/glm/detail/type_mat3x3.hpp>
#include "shape.hpp"


glm::vec3 Shape::get_translated_vec3(const glm::vec3 &vec3, const glm::vec3 &translation)const {
    return vec3 + translation;
}

glm::vec3 Shape::get_scaled_vec3(const glm::vec3 &vec3, float scale_x, float scale_y, float scale_z)const {
    glm::vec3 scaled = vec3;
    scaled[0] *= scale_x;
    scaled[1] *= scale_y;
    scaled[2] *= scale_z;

    return scaled;//glm::vec3 {scale_x,scale_y,scale_z} * vec3;
}

glm::vec3 Shape::get_rotated_vec3(const glm::vec3 &vec3, glm::vec3 axis, float angle)const {

    float sin_angle = (float) std::sin(angle);
    float cos_angle = (float) std::cos(angle);

    axis = glm::normalize(axis);

    glm::mat3x3 rotation_matrix{
            axis[0] * axis[0] * (1 - cos_angle) + cos_angle           ,axis[1] * axis[0] * (1 - cos_angle) - sin_angle * axis[2] ,axis[2] * axis[0] * (1 - cos_angle) + sin_angle * axis[1],
            axis[0] * axis[1] * (1 - cos_angle) + sin_angle * axis[2] ,axis[1] * axis[1] * (1 - cos_angle) + cos_angle           ,axis[2] * axis[1] * (1 - cos_angle) - sin_angle * axis[0] ,
            axis[0] * axis[2] * (1 - cos_angle) - sin_angle * axis[1] ,axis[1] * axis[2] * (1 - cos_angle) + sin_angle * axis[0] ,axis[2] * axis[2] * (1 - cos_angle) + cos_angle
    };


    return rotation_matrix * vec3;
}

void Shape::set_angle(float angle) {
    angle_ = angle;
}

void Shape::set_rotation_axis(glm::vec3 axis) {
    rotation_axis_ = axis;
}
