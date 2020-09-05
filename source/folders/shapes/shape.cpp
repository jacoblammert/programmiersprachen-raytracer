
#include <glm-0.9.5.3/glm/detail/type_mat3x3.hpp>
#include <glm-0.9.5.3/glm/detail/func_matrix.inl>
#include <iostream>
#include "shape.hpp"


glm::vec3 Shape::get_translated_vec3(const glm::vec3 &vec3, const glm::vec3 &translation)const {
    return vec3 + translation;
}

glm::vec3 Shape::get_scaled_vec3(const glm::vec3 &vec3, float scale_x, float scale_y, float scale_z)const {
    return glm::vec3 {scale_x,scale_y, scale_z} * vec3;
}

glm::vec3 Shape::get_rotated_vec3(const glm::vec3 &vec3, glm::vec3 axis, float angle)const {

    float sin_angle = std::sin(angle);
    float cos_angle = std::cos(angle);

    axis = glm::normalize(axis);

    glm::mat3x3 rotation_matrix = glm::mat3x3{
            axis[0] * axis[0] * (1 - cos_angle) + cos_angle           ,axis[1] * axis[0] * (1 - cos_angle) - sin_angle * axis[2] ,axis[2] * axis[0] * (1 - cos_angle) + sin_angle * axis[1],
            axis[0] * axis[1] * (1 - cos_angle) + sin_angle * axis[2] ,axis[1] * axis[1] * (1 - cos_angle) + cos_angle           ,axis[2] * axis[1] * (1 - cos_angle) - sin_angle * axis[0] ,
            axis[0] * axis[2] * (1 - cos_angle) - sin_angle * axis[1] ,axis[1] * axis[2] * (1 - cos_angle) + sin_angle * axis[0] ,axis[2] * axis[2] * (1 - cos_angle) + cos_angle
    };

    return rotation_matrix * vec3;
}

void Shape::set_angle(float angle) {
    angle_ = angle;
}

void Shape::set_rotation_axis(const glm::vec3& axis) {
    axis_ = glm::normalize(axis);

    glm::vec3 up_vec = glm::normalize(glm::vec3{0, 0, 1}); // vector used for rotation
    glm::vec3 rotation_axis = glm::normalize(glm::cross(up_vec, axis_));
    float angle = (float)acos(glm::dot(up_vec,axis_));




    float sin_angle = std::sin(angle);
    float cos_angle = std::cos(angle);


    rotation_matrix_ = glm::mat3x3{
            rotation_axis[0] * rotation_axis[0] * (1 - cos_angle) + cos_angle           ,rotation_axis[1] * rotation_axis[0] * (1 - cos_angle) - sin_angle * rotation_axis[2] ,rotation_axis[2] * rotation_axis[0] * (1 - cos_angle) + sin_angle * rotation_axis[1],
            rotation_axis[0] * rotation_axis[1] * (1 - cos_angle) + sin_angle * rotation_axis[2] ,rotation_axis[1] * rotation_axis[1] * (1 - cos_angle) + cos_angle           ,rotation_axis[2] * rotation_axis[1] * (1 - cos_angle) - sin_angle * rotation_axis[0] ,
            rotation_axis[0] * rotation_axis[2] * (1 - cos_angle) - sin_angle * rotation_axis[1] ,rotation_axis[1] * rotation_axis[2] * (1 - cos_angle) + sin_angle * rotation_axis[0] ,rotation_axis[2] * rotation_axis[2] * (1 - cos_angle) + cos_angle
    };

    sin_angle = std::sin(-angle);
    cos_angle = std::cos(-angle);
    rotation_matrix_inverse = glm::mat3x3{
            rotation_axis[0] * rotation_axis[0] * (1 - cos_angle) + cos_angle           ,rotation_axis[1] * rotation_axis[0] * (1 - cos_angle) - sin_angle * rotation_axis[2] ,rotation_axis[2] * rotation_axis[0] * (1 - cos_angle) + sin_angle * rotation_axis[1],
            rotation_axis[0] * rotation_axis[1] * (1 - cos_angle) + sin_angle * rotation_axis[2] ,rotation_axis[1] * rotation_axis[1] * (1 - cos_angle) + cos_angle           ,rotation_axis[2] * rotation_axis[1] * (1 - cos_angle) - sin_angle * rotation_axis[0] ,
            rotation_axis[0] * rotation_axis[2] * (1 - cos_angle) - sin_angle * rotation_axis[1] ,rotation_axis[1] * rotation_axis[2] * (1 - cos_angle) + sin_angle * rotation_axis[0] ,rotation_axis[2] * rotation_axis[2] * (1 - cos_angle) + cos_angle
    };
}

std::string Shape::get_name() const {
    return name_;
}

ShapeType Shape::get_shape_type() const {
    return shape_type_;
}
