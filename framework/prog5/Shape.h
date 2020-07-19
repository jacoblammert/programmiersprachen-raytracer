

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H


#include <glm-0.9.5.3/glm/vec3.hpp>
#include <string>
#include "color.hpp"

class Shape {

public:
    Shape(std::string name, Color color);

    virtual std::ostream &print(std::ostream &os) const;

    /**/virtual/**/ ~Shape();

protected:
    ///purely virtual protected functions for other shapes to implement
    virtual float area() const = 0;

    virtual float volume() const = 0;

    std::string name_;
    Color color_;

};

std::ostream & operator << ( std::ostream & os , Shape const &s);

#endif //RAYTRACER_SHAPE_H
