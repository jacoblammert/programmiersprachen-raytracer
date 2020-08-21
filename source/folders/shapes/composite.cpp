//
// Created by Jacob Lammert on 20.08.2020.
//

#include "composite.hpp"
#include "plane.hpp"

#include <utility>

/**
 * default constructor
 */
Composite::Composite() {

}

/**
 * Initializer with a depth
 * @param depth must be incremented, shapes will most likely be added after this constructor is called
 */
Composite::Composite(int depth) : depth{++depth} {

}


/**
 * Initializer with a vector of Shape pointers and a depth
 * @param shapes
 * @param depth
 */
Composite::Composite(std::vector<Shape *> shapes) :
        shapes{std::move(shapes)} {


    set_min_max_mid();

    if (!this->shapes.empty()) {
        split(); // it is not an endless loop
    }
}


/**
 * //Not usefull!!! must be implemented anyways
 * To check if the Compositeobject has been hit is to check, if the boxobject has been hit
 * Therefore we use the box.getIntersectVect(...) function
 * @param ray to be tested
 * @param HitPoint position where the ray intersects the box first (for two intersections the closest point will be returned) as reference
 * @param HitNormal The normal of the box at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the box is in front of the ray and has been intersected
 */
bool Composite::get_intersect_vec(const Ray &ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const {
    return box.get_intersect_vec(ray, HitPoint, HitNormal, distance);
}

/**
 * This function is generally used to get the Shapepointer of an intersected shape
 * as well as the position (glm::vec3), the normal (glm::vec3) at the position and the distance (float)
 * + boolean whether a shape has been hit or not
 *
 * @param ray to be tested
 * @param shape intersected shape as pointer
 * @param Hitpoint of intersection as glm::vec3
 * @param Hitnormal at intersection as glm::vec3 (normalized)
 * @param distance as float
 * @param hit boolean, false, if no shape has been intersected
 */
void
Composite::get_intersected_shape(const Ray &ray, Shape &shape, glm::vec3 &Hitpoint, glm::vec3 &Hitnormal, float &distance,
                               bool &hit) {

    for (auto &boxe : boxes) { // besser box als boxe?
        if (boxe.box.get_intersect(ray)) {
            boxe.get_intersected_shape(ray, shape, Hitpoint, Hitnormal, distance, hit);
        }
    }

    if (!shapes.empty()) {
        float oldDist = distance;
        for (auto &shapehit : shapes) {
            shapehit->get_intersect_vec(ray, Hitpoint, Hitnormal, distance);
            if (distance < oldDist) { // Test, if new already hit shape is closer and set shape to shape and hit to true
                shape = *shapehit;
                hit = true;
            }
        }
    }
}

/**
 * not usefull for composite objects, because they are build different
 * and never required to be drawn
 * @param pos
 * @return
 */
glm::vec3 Composite::get_normal(const glm::vec3 &pos) const {
    return glm::vec3();
}


/**
 * @return the min point of all the minpoints of all shapes contained in this composite object
 * smallest possible x,y,z for all shapes
 */
glm::vec3 Composite::get_min() const {
    return minXminYminZ;
}


/**
 * @return the max point of all the maxpoints of all shapes contained in this composite object
 * largest possible x,y,z for all shapes
 */
glm::vec3 Composite::get_max() const {
    return maxXmaxYmaxZ;
}

/**
 * @return the mid point of all the midpoints of all shapes contained in this composite object
 */
glm::vec3 Composite::get_median() const {
    return median;
}


/**
 * Do we want to translate each item?
 * //TODO do later, its gonna take some time
 * @param position
 */
void Composite::translate(const glm::vec3 &position) {

}

/**
 * Only really usefull, if there are not to many shapes in total, or the console will be filled with exponentially many
 * comments
 */
void Composite::print() const {
/*/
       std::cout << std::endl << "Depth: " << this->depth << std::endl;
    std::cout << "Size: " << shapes.size() << std::endl;
    std::cout << "MinX: " << minXminYminZ.get(0) << " MinY: " << minXminYminZ.get(1) << " MinZ: " << minXminYminZ.get(2)
              << std::endl;
    std::cout << "MaxX: " << maxXmaxYmaxZ.get(0) << " MaxY: " << maxXmaxYmaxZ.get(1) << " MaxZ: " << maxXmaxYmaxZ.get(2)
              << std::endl;
    std::cout << "MidX: " << median.get(0) << " MidY: " << median.get(1) << " MidZ: " << median.get(2) << std::endl
              << std::endl;
    if (depthToPrint > 0) {
        depthToPrint--;
        for (auto &boxe : boxes) {
            boxe.print(depthToPrint);
        }
    }
    /**/
}

/**
 * This function sets the minimum, the maximum and medium vector for the box
 */
void Composite::set_min_max_mid() {
    minXminYminZ = glm::vec3(INFINITY, INFINITY,
                             INFINITY); // need to be set to opposite values to get the correct ones for all the shapes inside this box
    maxXmaxYmaxZ = glm::vec3(-INFINITY, -INFINITY,
                             -INFINITY); // need to be set to opposite values to get the correct ones for all the shapes inside this box

    median = glm::vec3();

    glm::vec3 medianShape;

    for (auto &shape : shapes) {
        get_min(shape->get_min());
        get_max(shape->get_max());

        medianShape = shape->get_median();
        median = median + medianShape;
    }
    median *= (1.0f / (float) shapes.size());

    box = Box{minXminYminZ, maxXmaxYmaxZ};
}


/**
 * This function creates new Composite objects and fills them according to their midpoints
 * They are put either left or right, top or bottom, or front and back.
 * The new Compositeobjects fit into axisalligned Boxes and the Shapes are put either in the first or the later
 */
void Composite::split() {

    int axis;
    glm::vec3 size = maxXmaxYmaxZ - minXminYminZ; // max of the Box in terms of width, height, depth


    if (size[0] > size[1] && size[0] > size[2]) { // X is largest
        axis = 0; // x split
    } else if (size[1] > size[0] && size[1] > size[2]) { // Y is lagest
        axis = 1; // y split
    } else { // Z is largest
        axis = 2; // z split
    }

    // 1 0   0
    // 0 1   1
    // 0 0   2


    Composite left = Composite(depth);
    Composite right = Composite(depth);

    boxes.push_back(right);
    boxes.push_back(left);


    for (int i = (int) shapes.size() - 1; 0 < i; --i) {
        if (!dynamic_cast<Plane *>(shapes[i])) { // Planes stay in the first Box, because they are really big

            if (shapes[i]->get_median()[axis] > median[axis]) { // right
                boxes[0].add_shape(shapes[i]); // right
            } else {
                boxes[1].add_shape(shapes[i]);
            }

            shapes.erase(shapes.begin() + i);
        }
    }

    for (auto &boxe : boxes) {
        boxe.build();
    }
}

/**
 * The box is now filled with shapes which can be sorted into smaller ones
 * This function gets the new largest  & smallest values of the shapes inside the vector
 * and splits them up into new Compositeobjects, if the depth is greater than a given value, or the shape vector has reached a certain size
 */
void Composite::build() {
    set_min_max_mid();

    if (depth < 20 && shapes.size() > 8) { // TODO make better system to change these values
        split();
    }
}


/**
 * Changes the minXminYminZ vector, if either x, y or z of the vector shapemax is smaller
 * and sets the corresponding value to the newer, smaller one
 * @param shapemin vec3
 */
void Composite::get_min(glm::vec3 shapemin) {
    for (int i = 0; i < 3; ++i) {
        if (minXminYminZ[i] > shapemin[i]) {
            minXminYminZ[i] = shapemin[i];
        }
    }
}

/**
 * Changes the maxXmaxYmaxZ vector, if either x, y or z of the vector shapemax is bigger
 * and sets the corresponding value to the newer, larger one
 * @param shapemax vec3
 */
void Composite::get_max(glm::vec3 shapemax) {
    for (int i = 0; i < 3; ++i) {
        if (maxXmaxYmaxZ[i] < shapemax[i]) {
            maxXmaxYmaxZ[i] = shapemax[i];
        }
    }
}

/**
 * Adds a Shape pointer to the Compositeobject usefull, if the object has not been build yet
 * @param shape pointer
 */
void Composite::add_shape(Shape *shape) {
    shapes.push_back(shape);
}
/**
 * Adds a vector with Shape pointers to the Compositeobject usefull, if the object has not been build yet
 * @param shapes  = vector with Shape pointers
 */
void Composite::add_shapes(std::vector<Shape *> shapes) {
    //TODO addShape function
}








