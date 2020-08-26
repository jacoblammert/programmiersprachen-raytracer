#include "cone.hpp"
#include "plane.hpp"

bool Cone::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {



/// only for "normalized cone in origin with diameter 1 and height 1

/// Bottom of cone:
/**/


    float t = glm::dot(position_ - ray.position, rotation_axis_) / glm::dot(ray.direction, rotation_axis_);
    float length = INFINITY;

    /*/
    if (0 <= t && t < distance) {
        length = glm::length(ray.position - pos_ + ray.direction * t);

        if (length < (width_ / 2)) {
            distance = t;
            hit_point = ray.position + ray.direction * t;
            hit_normal = -rotation_axis_;
        } else {
            t = INFINITY;
        }
    } else {
        t = INFINITY;
    }/**/


/// other part of the cone:

/**/

    glm::vec3 up_vec = glm::normalize(glm::vec3{0.001, 0.001, 100});

    float angle_new = (float) 3.14159265359f * (1 - glm::dot(rotation_axis_,up_vec));

    glm::vec3 rotation_axis = glm::cross(up_vec, rotation_axis_);


    glm::vec3 ap_ = get_rotated_vec3(get_translated_vec3(ray.position, -position_), rotation_axis, angle_new);

///The ap_ and ad_ are now translated & rotated

    /// now scaling for width + height

    float inverse_width = (float)1.0f / width_;
    float inverse_height = (float)1.0f / height_;


    ap_ = get_scaled_vec3(ap_,inverse_width,inverse_width,inverse_height);



    glm::vec3 ad_ = get_rotated_vec3(ray.direction, rotation_axis, angle_new);
    ad_ = get_scaled_vec3(ad_,inverse_width,inverse_width,inverse_height);
    ad_ = glm::normalize(ad_);

    glm::vec3 theta = {0, 0, 1};
    float m = 0.25f;
    glm::vec3 w = ap_ - theta;

    float w_theta = glm::dot(w, theta);


    double a = glm::dot(ad_, ad_) - m * (pow(glm::dot(ad_, theta), 2)) - pow(glm::dot(ad_, theta), 2);
    double b = 2 * (glm::dot(ad_, w) - m * glm::dot(ad_, theta) * w_theta - glm::dot(ad_, theta) * w_theta);
    double c = glm::dot(w, w) - m * (w_theta * w_theta) - (w_theta * w_theta);

    double det = b * b - (4 * a * c);


    if (det > 0) {

        det = sqrt(det);
        c = (-b - det) / (2 * a);
        det = (-b + det) / (2 * a);

        if (c < 0 || det > 0 && det < c) {
            c = det;
        }
        if (c > 0) {

            glm::vec3 cp = ap_ + ad_ * (float) c;

            if (!(cp[2] < 0 || cp[2] > 1)) {

                if (0 < c && c < distance/*/ && c < t/**/) {
                    hit_point = get_rotated_vec3(cp, rotation_axis, -angle_new);
                    hit_point = get_scaled_vec3(hit_point, width_, width_, height_);

                    hit_point = get_translated_vec3(hit_point,rotation_axis_);

                    c = glm::length(hit_point - ray.position);


                    distance = (float) c;
                    hit_normal = cp * (cp[2] / glm::dot(cp, cp)) - glm::vec3{0, 0, 1};

                    hit_normal = get_rotated_vec3(hit_normal, rotation_axis, -angle_new);
                    hit_normal = glm::normalize(get_scaled_vec3(hit_normal,width_,width_,height_));





                    return true;
                }
            }
        }
    }/**/

    if (t < INFINITY && t < distance && 0 < t) {
        return true;
    }


    return false;
    /**/
}

glm::vec3 Cone::get_normal(const glm::vec3 &pos) const {
    return glm::normalize(pos - position_);
}

glm::vec3 Cone::get_min() const {
    return position_ - glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_);
}

glm::vec3 Cone::get_max() const {
    return position_ + glm::vec3{1, 1, 1} * (float) sqrt(height_ * height_ + width_ * width_);
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
}
