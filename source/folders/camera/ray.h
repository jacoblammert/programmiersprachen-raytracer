//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include <glm-0.9.5.3/glm/vec3.hpp>

struct ray {
    //TODO put in math folder + check if classname should be in caps/ ray or Ray  + direction  ust always be normalized
    glm::vec3 position;
    glm::vec3 direction;
};


#endif //RAYTRACER_RAY_H
