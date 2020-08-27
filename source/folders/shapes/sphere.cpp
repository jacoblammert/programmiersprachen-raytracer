#include "sphere.hpp"

/**
 * Sphere with a given Position and radius
 * @param position as glm::vec3 for x, y, z
 * @param radius value of input, if radius = -10, radius will be made positive
 */
Sphere::Sphere(const glm::vec3 &position, float radius) :
        radius_ {abs(radius)}
{
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

    float t = glm::dot(position_ - ray.position_, ray.direction_);
    float x = glm::length(position_ - ray.position_ - (ray.direction_ * t));

    if (x < radius_) {

        x = sqrt(radius_ * radius_ - x * x);

        // "improved" code by reusing variables & removing some for simplicity/ speed

        if (0 < (t - x) && (t - x) < distance) {
            distance = (t - x);
            hit_point = ray.position_ + (ray.direction_ * (t - x));
            hit_normal = get_normal(hit_point);
            return true;
        }

        if ((t - x) < 0 && 0 < (t + x) && (t + x) < distance) {
            distance = (t + x);
            hit_point = ray.position_ + (ray.direction_ * (t + x));
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
    //glm::vec3 rad = glm::vec3(radius_, radius_, radius_);
    glm::vec3 rad {radius_, radius_, radius_};
    return position_ - rad;
}


/**
 * @return a vector with the maximal values of x, y and z for the sphere (for a box around the sphere)
 */
glm::vec3 Sphere::get_max() const {
    //glm::vec3 rad = glm::vec3(radius_, radius_, radius_);
    glm::vec3 rad {radius_, radius_, radius_};
    return position_ + rad;
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
 * //TODO implement a good print function
 * outputs important information in the console
 */
void Sphere::print() const {
    std::cout << "Sphere" << std::endl;
    //print();
}

/**
 * Changes the position of the sphere with a given glm::vec3
 * @param position
 */
void Sphere::translate(glm::vec3 const& position) {
    position_ += position;
}
