//
// Created by Jacob Lammert on 20.08.2020.
//

#include "composite.hpp"

/**
 * default constructor
 */
Composite::Composite() {

}

/**
 * Initializer with a depth
 * @param depth must be incremented, shapes will most likely be added after this constructor is called
 */
Composite::Composite(int depth) :
    depth_ {++depth}
{}


/**
 * Initializer with a vector of Shape pointers and a depth
 * @param shapes
 * @param depth
 */
Composite::Composite(std::vector<std::shared_ptr<Shape>> const& shapes) :

    shapes_ {std::move(shapes)} {
        set_min_max_mid();

        if (!this->shapes_.empty()) {
            split(); // it is not an endless loop
        }
            
}


/**
 * //Not usefull!!! must be implemented anyways
 * To check if the Composite object has been hit is to check, if the boxobject has been hit
 * Therefore we use the box.getIntersectVect(...) function
 * @param ray to be tested
 * @param hit_point position where the ray intersects the box first (for two intersections the closest point will be returned) as reference
 * @param hit_normal The normal of the box at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the box is in front of the ray and has been intersected
 */
bool Composite::get_intersect_vec(const Ray &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) const {
    return box_.get_intersect_vec(ray, hit_point, hit_normal, distance);
}

/**
 * This function is generally used to get the Shapepointer of an intersected shape
 * as well as the position (glm::vec3), the normal (glm::vec3) at the position and the distance (float)
 * + boolean whether a shape has been hit or not
 *
 * @param ray to be tested
 * @param shape intersected shape as pointer
 * @param hit_point of intersection as glm::vec3
 * @param hit_normal at intersection as glm::vec3 (normalized)
 * @param distance as float
 * @param hit boolean, false, if no shape has been intersected
 */
void
Composite::get_intersected_shape(const Ray &ray, std::shared_ptr<Shape> & shape, glm::vec3 &hit_point, glm::vec3 &hit_normal, float &distance) {

    for (auto &box : boxes_) {
        if (box.box_.get_intersect(ray)) {
            box.get_intersected_shape(ray, shape, hit_point, hit_normal, distance);
        }
    }

    if (!shapes_.empty()) {
        for (auto &shape_hit : shapes_) {
            if (shape_hit->get_intersect_vec(ray, hit_point, hit_normal, distance)) {
                shape = shape_hit; // shape_hit is the closest intersected shape of all
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
    return box_.get_normal(pos);
}


/**
 * @return the min point of all the minpoints of all shapes contained in this composite object
 * smallest possible x,y,z for all shapes
 */
glm::vec3 Composite::get_min() const {
    return min_x_y_z_;
}


/**
 * @return the max point of all the maxpoints of all shapes contained in this composite object
 * largest possible x,y,z for all shapes
 */
glm::vec3 Composite::get_max() const {
    return max_x_y_z_;
}

/**
 * @return the mid point of all the midpoints of all shapes contained in this composite object
 */
glm::vec3 Composite::get_median() const {
    return median_;
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
    min_x_y_z_ = glm::vec3(INFINITY, INFINITY,
                             INFINITY); // need to be set to opposite values to get the correct ones for all the shapes inside this box
    max_x_y_z_ = glm::vec3(-INFINITY, -INFINITY,
                             -INFINITY); // need to be set to opposite values to get the correct ones for all the shapes inside this box

    median_ = glm::vec3();

    glm::vec3 median_shape;

    for (auto &shape : shapes_) {
        get_min(shape->get_min());
        get_max(shape->get_max());

        median_shape = shape->get_median();
        median_ = median_ + median_shape;
    }
    median_ *= (1.0f / (float) shapes_.size());

    box_ = Box{min_x_y_z_, max_x_y_z_};
}


/**
 * This function creates new Composite objects and fills them according to their midpoints
 * They are put either left or right, top or bottom, or front and back.
 * The new Compositeobjects fit into axisalligned Boxes and the Shapes are put either in the first or the later
 */
void Composite::split() {

    int axis;
    glm::vec3 size = max_x_y_z_ - min_x_y_z_; // max of the Box in terms of width, height, depth


    if (size[0] > size[1] && size[0] > size[2]) { // X is largest
        axis = 0; // x split
    } else if (size[1] > size[0] && size[1] > size[2]) { // Y is lagest
        axis = 1; // y split
    } else { // Z is largest
        axis = 2; // z split
    }


    Composite left = Composite(depth_);
    Composite right = Composite(depth_);

    boxes_ = std::vector<Composite>(2);

    boxes_[0] = (right);
    boxes_[1] = (left);

    for (int i = (int) shapes_.size() - 1; 0 <= i; --i) {
        //std::cout<<i << " " << count++ <<" Type: " << typeid(*shapes[i].get()).hash_code()<<std::endl;
        /// hashcode must not be equal to the planes hashcode
        //TODO check, if a hashcode might be different on different systems
        if (typeid(*shapes_[i].get()).hash_code() != 3060751613) { // Planes stay in the first Box, because they are really big

            if (shapes_[i]->get_median()[axis] > median_[axis]) { // right
                boxes_[0].add_shape(shapes_[i]); // right
            } else {
                boxes_[1].add_shape(shapes_[i]);
            }
            shapes_.erase(shapes_.begin() + i);
        }
    }

    for (auto &box : boxes_) {
        box.build();
    }
}

/**
 * The box is now filled with shapes which can be sorted into smaller ones
 * This function gets the new largest  & smallest values of the shapes inside the vector
 * and splits them up into new Compositeobjects, if the depth is greater than a given value, or the shape vector has reached a certain size
 */
void Composite::build() {
    set_min_max_mid();

    if (depth_ < 20 && shapes_.size() >= 4) { // TODO make better system to change these values
        split();
    }
}


/**
 * Changes the minXminYminZ vector, if either x, y or z of the vector shapemax is smaller
 * and sets the corresponding value to the newer, smaller one
 * @param shapemin vec3
 */
void Composite::get_min(glm::vec3 const& shape_min) {
    for (int i = 0; i < 3; ++i) {
        if (min_x_y_z_[i] > shape_min[i]) {
            min_x_y_z_[i] = shape_min[i];
        }
    }
}

/**
 * Changes the maxXmaxYmaxZ vector, if either x, y or z of the vector shapemax is bigger
 * and sets the corresponding value to the newer, larger one
 * @param shapemax vec3
 */
void Composite::get_max(glm::vec3 const& shape_max) {
    for (int i = 0; i < 3; ++i) {
        if (max_x_y_z_[i] < shape_max[i]) {
            max_x_y_z_[i] = shape_max[i];
        }
    }
}

/**
 * Adds a Shape pointer to the Compositeobject usefull, if the object has not been build yet
 * @param shape pointer
 */
void Composite::add_shape(const std::shared_ptr<Shape>& shape) {
    shapes_.push_back(shape);
}
/**
 * Adds a vector with Shape pointers to the Compositeobject usefull, if the object has not been build yet
 * @param shapes  = vector with Shape pointers
 */
void Composite::add_shapes(std::vector<std::shared_ptr<Shape>> const& new_shapes) {
    this->shapes_.insert(this->shapes_.end(),new_shapes.begin(),new_shapes.end());
}








