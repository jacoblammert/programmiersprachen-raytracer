


#include "Sphere.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>


/***
 * Constructor using the Constructor of the base class
 * @param center center position in 3d space
 * @param radius radius as float, negative values will be made positive
 * @param name as String, using Shape constructor
 * @param color as Color, using Shape constructor
 */
Sphere::Sphere(glm::vec3 center, float radius, std::string name, Color color):
center_(center),radius_(abs(radius)), Shape(name,color){
}
/**
 * @return area as float uses M_PI constant from <math.h> to compute the total area
 */
float Sphere::area() const{
    return 4 * M_PI * radius_;
}

/**
 * @return volume of the sphere as float uses M_PI constant from <math.h>
 */
float Sphere::volume() const{
    return (float)(4 * M_PI * pow(radius_,3))/3;
}

std::ostream &Sphere::print(std::ostream &os) const {
    Shape::print(os);

    os<<"Center: "<<center_[0]<<" , " <<center_[1]<<" , " <<center_[2]<<"\nRadius: "<<radius_<<"\n";
    os<<"Area: " << area() << "\nVolume: "<<volume()<<"\n\n";
    return os;
}




