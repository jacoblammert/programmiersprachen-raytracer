
#include "Shape.h"



/**
 * Constructor used by derived classes
 * @param name as string
 * @param color as color for r,g,b
 */
Shape::Shape(std::string name, Color color):
        name_{name},color_(color){}
