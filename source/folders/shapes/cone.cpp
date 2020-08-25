#include "cone.hpp"
#include "plane.hpp"

bool Cone::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {



/// only for "normalized cone in origin with diameter 1 and height 1

/// Bottom of cone:
    Plane p{pos_,{0,0,-1}};

    p.set_material({});

    glm::vec3 newnormal;
    glm::vec3 newhit{INFINITY,INFINITY,INFINITY};
    float newdist = INFINITY;

    if (p.get_intersect_vec(ray,newhit,newnormal,newdist) && glm::length(newhit - pos_) <= 0.5f) {

        if (0 < newdist &&  newdist < distance && glm::dot(newnormal, ray.direction) < 0) {
            distance = newdist;
            hit_point = ray.position + ray.direction * newnormal;
            hit_normal = newnormal;
            return true;
        }
    }

/// other parts of the cone:


    glm::vec3 ap_ = ray.position;
    glm::vec3 ad_ = ray.direction;

    glm::vec3 theta = glm::normalize(axis_);
    float m = pow(angle_,2)/pow(height_,2);
    glm::vec3 w = ap_ - pos_ - theta * height_;

    float a = glm::dot(ad_,ad_) - m * (pow(glm::dot(ad_,theta),2)) - pow(glm::dot(ad_,theta),2);
    float b = 2.f * ( glm::dot(ad_,w) - m * glm::dot(ad_,theta)*glm::dot(w,theta) - glm::dot(ad_,theta)*glm::dot(w,theta) );
    float c = glm::dot(w,w) - m * pow(glm::dot(w,theta),2) - pow(glm::dot(w,theta),2);

    float det = pow(b,2) - (4.f * a * c);


    if (det < 0) return false;

    det = sqrt(det);
    float t1 = (-b - det) / (2 * a);
    float t2 = (-b + det) / (2 * a);

    if (t1 < 0 || t2 > 0 && t2 < t1){
        t1 = t2;
    }
    if (t1 < 0) return false;

    glm::vec3 cp = ray.position + t1 * ray.direction - pos_;

    if (glm::dot(cp, axis_) < 0 || glm::dot(cp, axis_) > height_) return false;
    if (0 < t1 && t1 < distance) {
        distance = t1;
        hit_point = ray.position + ray.direction * t1;
        hit_normal = glm::normalize(cp * (glm::dot(axis_, cp) / glm::dot(cp, cp)) - axis_);

        return true;
    }
    return false;
}

glm::vec3 Cone::get_normal(const glm::vec3 &pos) const {
    return glm::normalize(pos - pos_);
}

glm::vec3 Cone::get_min() const {
    return pos_ - glm::vec3{1, 1, 1} * (height_ * 3);
}

glm::vec3 Cone::get_max() const {
    return pos_ + glm::vec3{1, 1, 1} * (height_ * 3);
}

glm::vec3 Cone::get_median() const {
    return pos_;
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
    pos_ += position;
}

Cone::Cone(const glm::vec3 &position, const glm::vec3 &direction, float angle, float height) :
        pos_{position}, axis_{direction}, angle_{angle}, height_{height} {

}
