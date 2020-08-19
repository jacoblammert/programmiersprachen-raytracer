//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include <glm-0.9.5.3/glm/geometric.hpp>

struct Ray {
    //TODO put in math folder direction  (remember) must always be normalized
    glm::vec3 position;
    glm::vec3 direction;
    Ray(glm::vec3 position,glm::vec3 direction);
};


#endif //RAYTRACER_RAY_H
