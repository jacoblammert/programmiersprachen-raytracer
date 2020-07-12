

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H


#include <glm-0.9.5.3/glm/vec3.hpp>

class Shape {
protected:
    ///purely virtual protected functions for other shapes to implement
    virtual const float area() = 0 ;
    virtual const float volume() = 0;

};


#endif //RAYTRACER_SHAPE_H
