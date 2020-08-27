#include <cmath>
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

    //std::cout<<"Old vec3: \n";
    //std::cout << "X: " << axis[0] << " Y: " << axis[1] << " Z: " << axis[2] << std::endl;

    //float length = glm::length(vec3);

    float sin_angle = std::sin(angle);
    float cos_angle = std::cos(angle);

    //axis = glm::normalize(axis);

    glm::mat3x3 rotation_matrix = glm::mat3x3{
            axis[0] * axis[0] * (1 - cos_angle) + cos_angle           ,axis[1] * axis[0] * (1 - cos_angle) - sin_angle * axis[2] ,axis[2] * axis[0] * (1 - cos_angle) + sin_angle * axis[1],
            axis[0] * axis[1] * (1 - cos_angle) + sin_angle * axis[2] ,axis[1] * axis[1] * (1 - cos_angle) + cos_angle           ,axis[2] * axis[1] * (1 - cos_angle) - sin_angle * axis[0] ,
            axis[0] * axis[2] * (1 - cos_angle) - sin_angle * axis[1] ,axis[1] * axis[2] * (1 - cos_angle) + sin_angle * axis[0] ,axis[2] * axis[2] * (1 - cos_angle) + cos_angle
    };


    //std::cout << "Matrix: "<< std::endl;
    //std::cout << "X0: " << rotation_matrix[0][0] << " Y0: " << rotation_matrix[1][0] << " Z0: " << rotation_matrix[2][0] << std::endl;
    //std::cout << "X1: " << rotation_matrix[0][1] << " Y1: " << rotation_matrix[1][1] << " Z1: " << rotation_matrix[2][1] << std::endl;
    //std::cout << "X2: " << rotation_matrix[0][2] << " Y2: " << rotation_matrix[1][2] << " Z2: " << rotation_matrix[2][2] << std::endl;


    //float x = rotation_matrix[0][0] * axis[0] + rotation_matrix[1][0]  * axis[1] + rotation_matrix[2][0]  * axis[2];
    //float y = rotation_matrix[0][1] * axis[0] + rotation_matrix[1][1]  * axis[1] + rotation_matrix[2][1]  * axis[2];
    //float z = rotation_matrix[0][2] * axis[0] + rotation_matrix[1][2]  * axis[1] + rotation_matrix[2][2]  * axis[2];

    //axis = glm::vec3 {x,y,z};
    //std::cout<<"New vec3: \n";
    //std::cout << "X: " << axis[0] << " Y: " << axis[1] << " Z: " << axis[2] << std::endl;
    return rotation_matrix * vec3;//glm::vec3 {rotation_matrix[0][0] * axis[0] + rotation_matrix[1][0]  * axis[1] + rotation_matrix[2][0]  * axis[2],
            //          rotation_matrix[0][1] * axis[0] + rotation_matrix[1][1]  * axis[1] + rotation_matrix[2][1]  * axis[2],
            //          rotation_matrix[0][2] * axis[0] + rotation_matrix[1][2]  * axis[1] + rotation_matrix[2][2]  * axis[2]};
}

void Shape::set_angle(float angle) {
    angle_ = angle;
}

void Shape::set_rotation_axis(const glm::vec3& axis) {
    rotation_axis_ = axis;
}
