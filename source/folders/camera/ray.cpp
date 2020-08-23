
#include "ray.hpp"

/**
 * Added the .cpp to normalize the ray direction on creation
 * @param position of the ray
 * @param direction of the ray (will be normalized)
 */
Ray::Ray(const glm::vec3& position, const glm::vec3& direction):
position{position},direction{glm::normalize(direction)}
{}
