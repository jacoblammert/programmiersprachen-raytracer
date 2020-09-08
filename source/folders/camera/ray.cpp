#include "ray.hpp"

/**
 * Constructor to normalize the ray direction on creation
 * @param position of the ray
 * @param direction of the ray (will be normalized)
 */
Ray::Ray (glm::vec3 const& position, glm::vec3 const& direction):
    position {position},
    direction {glm::normalize(direction)}
{}
