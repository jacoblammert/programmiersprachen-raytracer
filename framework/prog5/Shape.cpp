
#include "Shape.h"


/**
 * Constructor used by derived classes
 * @param name as string
 * @param color as color for r,g,b
 */
Shape::Shape(std::string name, Color color) :
        name_{name}, color_(color) {}

std::ostream &Shape::print(std::ostream &os) const {
    os << "Name: " << name_ << "\nColor: " << color_.r << " , " << color_.g << " , " << color_.b << "\n";
    return os;
}

std::ostream& operator << (std::ostream& os, Shape const& s) {
    return s.print(os);
}
