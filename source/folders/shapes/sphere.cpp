#include "sphere.hpp"

/**
 * Sphere with a given Position and radius
 * @name name of sphere
 * @param position as glm::vec3 for x, y, z
 * @param radius value of input, if radius = -10, radius will be made positive
 */
Sphere::Sphere(std::string const& name, const glm::vec3 &position, float radius) :
        radius_ {abs(radius)}
{
    shape_type_ = SPHERE;
    name_ = name;
    position_ = position;

}

/**
 * returns true, if the sphere was in front of the ray and the ray intersected the sphere
 * @param ray to be tested
 * @param HitPoint position where the ray intersects the sphere first (for two intersections the closest point will be returned) as reference
 * @param HitNormal The normal of the sphere at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the sphere is in front of the ray and has been intersected
 */
bool Sphere::get_intersect_vec(Ray const& ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {

    float t = glm::dot(position_ - ray.position, ray.direction);
    float x = glm::length(position_ - ray.position - (ray.direction * t));

    if (x < radius_) {

        x = sqrt(radius_ * radius_ - x * x);

        // "improved/ faster" code by reusing variables & removing some for simplicity/ speed

        if (0 < (t - x) && (t - x) < distance) {
            distance = (t - x);
            hit_point = ray.position + (ray.direction * (t - x));
            hit_normal = get_normal(hit_point);
            return true;
        }

        if ((t - x) < 0 && 0 < (t + x) && (t + x) < distance) {
            distance = (t + x);
            hit_point = ray.position + (ray.direction * (t + x));
            hit_normal = get_normal(hit_point);
            return true;
        }
    }
    return false;
}

/**
 * returns the normal for a given position
 * @param position position (Hitposition)
 * @return normal (Vector from the Center to the input vector)
 */
glm::vec3 Sphere::get_normal(glm::vec3 const& position) const {
    return glm::normalize(position - this->position_);
}

/**
 * @return a vector with the minimal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::get_min() const {
    return position_ - glm::vec3{1, 1, 1} * radius_;
}


/**
 * @return a vector with the maximal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::get_max() const {
    return position_ + glm::vec3{1, 1, 1} * radius_;
}

/**
 * @return position of the sphere
 */
glm::vec3 Sphere::get_median() const {
    return position_;
}

/**
* @return material of the sphere
*/
std::shared_ptr<Material> Sphere::get_material() {
    return material_;
}

/**
* @param material is given to sphere
*/
void Sphere::set_material(std::shared_ptr<Material> const& material) {
    this->material_ = std::move(material);
}

/**
 * Changes the position of the sphere with a given glm::vec3
 * @param position
 */
void Sphere::translate(glm::vec3 const& position) {
    position_ += position;
}

/**
*Gives information of the spere
* @returns string with name and data
*/
std::string Sphere::get_information() const {
    std::string information = name_ + " " + std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " + std::to_string(position_[2]) + " " + std::to_string(radius_) + " " + material_->name;
    return information;
}
