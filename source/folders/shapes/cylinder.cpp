
#include "cylinder.hpp"

Cylinder::Cylinder(const glm::vec3 &position, const glm::vec3 &axis, float width, float height) :
        width_{width}, height_{height} {
    position_ = position;
    rotation_axis_ = axis;
}

bool Cylinder::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {

    //TODO translate, rotate & scale ray



    glm::vec3 axis = glm::vec3{0,0,1};

    float distance_0 = glm::dot(position_ - ray.position_, axis) / glm::dot(ray.direction_, axis);
    float length_0 = glm::length(ray.position_ - position_ + ray.direction_ * distance_0);

    bool hit_0 = length_0 < width_ / 2 && distance_0 < distance && 0 < distance_0;


    float distance_1 = glm::dot(position_ - ray.position_ + axis * height_, axis) /glm::dot(ray.direction_, axis);
    float length_1 = glm::length(ray.position_ - position_ - axis * height_ + ray.direction_ * distance_1);

    bool hit_1 = length_1 < width_ / 2 && distance_1 < distance && 0 < distance_1;




    float distance_2;
    bool hit_2 = false;


    glm::vec3 p0 = ray.position_ - position_;


    double a = ray.direction_[0] * ray.direction_[0] + ray.direction_[1] * ray.direction_[1];
    double b = ray.direction_[0] * p0[0] + ray.direction_[1] * p0[1];
    double c = p0[0] * p0[0] + p0[1] * p0[1] - (width_ / 2) * (width_ / 2);

    double delta = b * b - a * c;

    //use epsilon because of computation errors between doubles
    double epsilon = 0.00000001;

    // nearest intersection
    distance_2 = (float)((-b - sqrt(delta)) / a);
    float dist_3 = (float)((-b + sqrt(delta)) / a);

    distance_2 = 0 < dist_3  && (dist_3 < distance_2 ||  distance_2 < 0) ? dist_3 : distance_2;

        // t<0 means the intersection is behind the ray origin
        // which we don't want
        if (distance_2 >= epsilon) {


            float y = p0[2] + distance_2 * ray.direction_[2];

            // check if we intersect one of the bases
            if (!(y > height_ + epsilon || y < -epsilon)) {
                hit_2 = true;
            }
        }

    if (!hit_0){
        distance_0 = INFINITY;
    }
    if (!hit_1){
        distance_1 = INFINITY;
    }
    if (!hit_2){
        distance_2 = INFINITY;
    }

    if (hit_0 || hit_1 || hit_2){

        if (hit_0 && distance_0 < distance_1 && distance_0 < distance_2 && distance_0 < distance){
            //TODO reverse transformations
            hit_point = ray.position_ + ray.direction_ * distance_0;
            distance = distance_0;
            hit_normal = glm::vec3 {0,0,-1};
        } else if (hit_1 &&  distance_1 < distance_0 && distance_1 < distance_2 && distance_1 < distance){
            hit_point = ray.position_ + ray.direction_ * distance_1;
            distance = distance_1;
            hit_normal = glm::vec3 {0,0,1};
        } else if (distance_2 < distance){
            hit_point = ray.position_ + ray.direction_ * distance_2;
            distance = distance_2;
            hit_normal = get_normal(hit_point);
        } else{
            return false;
        }
        return true;
    }
    return false;
}

glm::vec3 Cylinder::get_normal(const glm::vec3 &pos) const {
    // Point is on one of the bases
    if (pos[0] < position_[0] + width_/2 && pos[0] > position_[0] - width_/2 && pos[1] < position_[1] + width_/2 && pos[1] > position_[1] - width_/2 ){
        double epsilon = 0.00000001;
        if (pos[2] < position_[2] + height_ + epsilon && pos[2] > position_[2] + height_-epsilon){
            return glm::vec3{0,0,1};
        }
        if (pos[2] < position_[2] + epsilon && pos[2] > position_[2]-epsilon){
            return glm::vec3 {0,0,-1};
        }
    }

    // Point is on lateral surface
    glm::vec3 c0 = glm::vec3 {position_[0], position_[1], pos[2]};
    return glm::normalize(pos - c0);
}

glm::vec3 Cylinder::get_min() const {
    return position_ - (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
}

glm::vec3 Cylinder::get_max() const {
    return position_ + (glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_));
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

void Cylinder::print() const {

}

void Cylinder::translate(const glm::vec3 &position) {
    position_ += position;
}
