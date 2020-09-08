#include "light.hpp"


/**
 * @param position as vec 3
 * @param color as vec 3 (red, gren, blue)
 * @param brightness as float
 * @param hardness as float => 1 = hard shadows, 0 = really soft shadows
 */

   Light::Light (std::string  name, glm::vec3 const& position, glm::vec3 const& color, glm::vec3 const& brightness, float hardness):
       name {std::move(name)},
       position {position},
       color {color},
       brightness {brightness},
       hardness {hardness}
   {}
