
#include "cylinder.hpp"

Cylinder::Cylinder(std::string const& name, const glm::vec3 &position, const glm::vec3 &axis, float width, float height) :
    width_ {width},
    height_ {height}
{
    shape_type_ = CYLINDER;
    name_ = name;
    position_ = position;
    set_rotation_axis(axis);
}

bool Cylinder::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {


    float distance_0 = glm::dot(position_ - ray.position_, axis_) / glm::dot(ray.direction_, axis_);
    float length_0 = glm::length(ray.position_ - position_ + ray.direction_ * distance_0);

    bool hit_0 = length_0 < width_ / 2 && distance_0 < distance && 0 < distance_0;


    float distance_1 = glm::dot(position_ - ray.position_ + axis_ * height_, axis_) /glm::dot(ray.direction_, axis_);
    float length_1 = glm::length(ray.position_ - position_ - axis_ * height_ + ray.direction_ * distance_1);

    bool hit_1 = length_1 < width_ / 2 && distance_1 < distance && 0 < distance_1;



    glm::vec3 ray_position = ray.position_;
    glm::vec3 ray_direction = ray.direction_;


    /// Translation
    ray_position = ray_position - position_;


    /// Rotation
    glm::vec3 up_vec = glm::vec3{0, 0, 1};
    ray_position = rotation_matrix_ * ray_position;

    ray_direction = rotation_matrix_ * ray_direction;


    /// Scalierung
    float inverse_width = 1.0f / width_;
    float inverse_height =  1.0f / height_;

    ray_position = get_scaled_vec3(ray_position, inverse_width ,  inverse_width,inverse_height);

    ray_direction = get_scaled_vec3(ray_direction, inverse_width,  inverse_width,inverse_height);
    ray_direction = glm::normalize(ray_direction);





    float distance_2;
    bool hit_2 = false;




    double a = ray_direction[0] * ray_direction[0] + ray_direction[1] * ray_direction[1];
    double b = ray_direction[0] * ray_position[0] + ray_direction[1] * ray_position[1];
    double c = ray_position[0] * ray_position[0] + ray_position[1] * ray_position[1] - (1.0f / 2) * (1.0f / 2);

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


            float y = ray_position[2] + distance_2 * ray_direction[2];

            // check if we intersect one of the bases
            if (!(y > 1 + epsilon || y < -epsilon)) {
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
    } else{

        hit_point = ray_position + ray_direction * distance_2;
        hit_normal = get_normal(hit_point);//{hit_point[0],0,hit_point[2]};//glm::normalize(glm::vec3{hit_point[0],0,hit_point[2]});//get_normal(hit_point);


        hit_point = get_scaled_vec3(hit_point, width_,  width_,height_);   /// first scaling
        hit_point = rotation_matrix_inverse * hit_point;/// second rotation
        hit_point = hit_point + position_;                                 /// third translation


        distance_2 = glm::length(ray.position_ - hit_point);
    }



    if (hit_0 || hit_1 || hit_2){

        if (hit_0 && distance_0 < distance_1 && distance_0 < distance_2 && distance_0 < distance){
            hit_point = ray.position_ + ray.direction_ * distance_0;
            distance = distance_0;
            hit_normal = glm::vec3 {0,0,-1};
        } else if (hit_1 &&  distance_1 < distance_0 && distance_1 < distance_2 && distance_1 < distance){
            hit_point = ray.position_ + ray.direction_ * distance_1;
            distance = distance_1;
            hit_normal = glm::vec3 {0,0,1};
        } else if (distance_2 < distance){
            distance = distance_2;
        } else{
            return false;
        }
        //TODO reverse transformations


        hit_normal = get_scaled_vec3(hit_normal, width_,  width_,height_);   /// first scaling
        hit_normal = rotation_matrix_inverse * hit_normal;/// second rotation
        hit_normal = glm::normalize(hit_normal);
        return true;
    }
    return false;
}

glm::vec3 Cylinder::get_normal(const glm::vec3 &pos) const {

    // Point is on lateral surface
    //glm::vec3 c0 = glm::vec3 {position_[0],pos[1] ,position_[2]};
    //return glm::normalize(pos - c0);
    glm::vec3 c0 = glm::vec3 {/*position_[0]*/0, 0/*position_[1]*/, pos[2]};
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
