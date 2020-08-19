
#include "ray.hpp"

/**
 * Added the .cpp to normalize the ray direction on creation
 * @param position of the ray
 * @param direction of the ray (will be normalized)
 */
Ray::Ray(glm::vec3 position, glm::vec3 direction):
position{position},direction{direction}{
    glm::normalize(this->direction);
}
