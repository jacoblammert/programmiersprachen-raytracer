#include "box.hpp"


/**
 * default initializer
 */
Box::Box() {

}

/**
 * box constructor for axis alligned box with a min and max vector for example: {-1,-1,-1} {1,1,1}
 * @param name name of box
 * @param minXminYminZ minimum of the box (individual values must be smaller than their counterpart in max vector)
 * @param maxXmaxYmaxZ maximum of the box (individual values must be greater than their counterpart in min vector)
 */
Box::Box(std::string const& name, const glm::vec3 &min_x_y_z, const glm::vec3 &max_x_y_z) {
    name_ = name;
    shape_type_ = BOX;
    position_ = (min_x_y_z + max_x_y_z) * 0.5f;
    bounds_.push_back(min_x_y_z);
    bounds_.push_back(max_x_y_z);
}

Box::Box(const glm::vec3 &min_x_y_z, const glm::vec3 &max_x_y_z) {
    position_ = (min_x_y_z + max_x_y_z) * 0.5f;
    bounds_.push_back(min_x_y_z);
    bounds_.push_back(max_x_y_z);
}

/**
 * box constructor for axis alligned box with a pos vector (middle of the box) and sizes for x, y and z as floats
 * @param Pos position (middle) of the box
 * @param xScale float for size in x direction
 * @param yScale float for size in y direction
 * @param zScale float for size in z direction
 */
Box::Box(glm::vec3 const &pos, float xScale, float yScale, float zScale){
    position_ = pos;

    glm::vec3 min_x_y_z = {pos[0] - (xScale / 2), pos[1] - (yScale / 2), pos[2] - (zScale / 2)};
    glm::vec3 max_x_y_z = {pos[0] + (xScale / 2), pos[1] + (yScale / 2), pos[2] + (zScale / 2)};
    bounds_.push_back(min_x_y_z);
    bounds_.push_back(max_x_y_z);
}
/* //TODO additional constructors with materials
Box::Box(Vector const& minXminYminZ, Vector const& maxXmaxYmaxZ, Color const& color) {
    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);

    position = minXminYminZ + maxXmaxYmaxZ;
    position.scale(0.5);
}

Box::Box(Vector const& Pos, float xScale, float yScale, float zScale, Color const& color) {
    Vector minXminYminZ = Vector(Pos.getX() - xScale / 2, Pos.getY() - yScale / 2, Pos.getZ() - zScale / 2);
    Vector maxXmaxYmaxZ = Vector(Pos.getX() + xScale / 2, Pos.getY() + yScale / 2, Pos.getZ() + zScale / 2);
    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);
    position = Pos; //vorherigen Konstruktor nutzen?
}*/


/**
 * returns true, if the box was in front of the ray and the ray intersected the box
 * @param ray to be tested
 * @param hit_point position where the ray intersects the box first (for two intersections the closest point will be returned) as reference
 * @param hit_normal The normal of the box at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the box is in front of the ray and has been intersected
 */
bool Box::get_intersect_vec(Ray const &ray, glm::vec3 &hit_point, glm::vec3 &hit_normal,
                            float &distance) const {

    glm::vec3 ray_direction = {1.0f / ray.direction_[0], 1.0f / ray.direction_[1], 1.0f / ray.direction_[2]};

    float t_min, t_max, t_y_min, t_y_max;

    /// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

    t_min = (bounds_[sign(ray_direction, 0)][0] - ray.position_[0]) * ray_direction[0]; //txmin?
    t_max = (bounds_[1 - sign(ray_direction, 0)][0] - ray.position_[0]) * ray_direction[0]; //tymin?

    t_y_min = (bounds_[sign(ray_direction, 1)][1] - ray.position_[1]) * ray_direction[1];
    t_y_max = (bounds_[1 - sign(ray_direction, 1)][1] - ray.position_[1]) * ray_direction[1];

    if ((t_min > t_y_max) || (t_y_min > t_max))
        return false;
    if (t_y_min > t_min)
        t_min = t_y_min;
    if (t_y_max < t_max)
        t_max = t_y_max;

    float t_z_min = (bounds_[sign(ray_direction, 2)][2] - ray.position_[2]) * ray_direction[2];
    float t_z_max = (bounds_[1 - sign(ray_direction, 2)][2] - ray.position_[2]) * ray_direction[2];

    if ((t_min > t_z_max) || (t_z_min > t_max))
        return false;
    if (t_z_min > t_min)
        t_min = t_z_min;
    if (t_z_max < t_max)
        t_max = t_z_max;


    if (0 < t_min && t_min < distance) {
        ray_direction = ray.direction_;
        ray_direction *= t_min;

        distance = t_min;
        hit_point = ray.position_ + ray_direction;
        hit_normal = get_normal(hit_point - position_);
        return true;
    }
    if (t_min < 0 && 0 < t_max && t_max < distance) {
        ray_direction = ray.direction_;
        ray_direction *= t_max;

        distance = t_max;
        hit_point = ray.position_ + ray_direction;
        hit_normal = get_normal(hit_point - position_);
        return true;
    }

    return false;
}


/**
 * Important for BoundingBox intersection testing. Similar to the previous method, but without all the useless stuff (could be faster)
 * @param ray that gets tested against this box
 * @return true if the ray hits
 */
bool Box::get_intersect(const Ray &ray) const {


    if (bounds_[0][0] < ray.position_[0] && ray.position_[0] < bounds_[1][0] &&
        bounds_[0][1] < ray.position_[1] && ray.position_[1] < bounds_[1][1] &&
        bounds_[0][2] < ray.position_[2] && ray.position_[2] < bounds_[1][2]) {
        /// The position of the ray is inside of the box, therefore it intersects the box
        return true;
    }

    glm::vec3 ray_direction = {1 / ray.direction_[0], 1 / ray.direction_[1], 1 / ray.direction_[2]};


    float t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;

    t_min = (bounds_[sign(ray_direction, 0)][0] - ray.position_[0]) * ray_direction[0];
    t_max = (bounds_[1 - sign(ray_direction, 0)][0] - ray.position_[0]) * ray_direction[0];

    t_y_min = (bounds_[sign(ray_direction, 1)][1] - ray.position_[1]) * ray_direction[1];
    t_y_max = (bounds_[1 - sign(ray_direction, 1)][1] - ray.position_[1]) * ray_direction[1];

    if ((t_min > t_y_max) || (t_y_min > t_max))
        return false;


    if (t_y_min > t_min)
        t_min = t_y_min;


    if (t_y_max < t_max)
        t_max = t_y_max;


    t_z_min = (bounds_[sign(ray_direction, 2)][2] - ray.position_[2]) * ray_direction[2];
    t_z_max = (bounds_[1 - sign(ray_direction, 2)][2] - ray.position_[2]) * ray_direction[2];

    return !((t_min > t_z_max) || (t_z_min > t_max));
}

/**
 * returns the normal for a given position
 * //TODO all the normals are pointing away from the cube. make a function to return the ones pointing "towards" a position (image hypercube)
 * @param position position (Hitposition)
 * @return normal at the given position. Either {1,0,0},{0,1,0},{0,0,1} or {-1,0,0},{0,-1,0},{0,0,-1}
 */
glm::vec3 Box::get_normal(glm::vec3 const &pos) const {

    glm::vec3 size = get_max() - get_min();

    size = pos/size; // with this transformation it works for different sized boxes (no rotaterino yet)


    if (fabs(size[0]) > fabs(size[1]) && fabs(size[0]) > fabs(size[2])) { // X is largest;
        return glm::vec3{1, 0, 0} * glm::sign(size[0]);
    } else if (fabs(size[1]) > fabs(size[2])) { // Y is largest
        return glm::vec3{0, 1, 0} * glm::sign(size[1]);
    } else {
        return glm::vec3{0, 0, 1} * glm::sign(size[2]);
    }
}


/**
 * @return a vector with the minimal values of x, y and z of the box
 */
glm::vec3 Box::get_min() const {
    return bounds_[0];
}


/**
 * @return a vector with the maximal values of x, y and z of the box
 */
glm::vec3 Box::get_max() const {
    return bounds_[1];
}

/**
 * @return the position of the box
 */
glm::vec3
Box::get_median() const { //besser getPosition? Ja, aber diese Funktion wird von verschiedenen Shapes genutzt z.B. Dreieck -> welche Position ist dann gemeint?
    // Und es gibt ja eine min/max Funktion -> mid/Median/M... Funktion macht nur Sinn
    return position_;
}

/**
 * outputs important information in the console
 */
void Box::print() const {
    std::cout << "Box: " << std::endl;
    std::cout << "Min: ";
    //bounds[0].print();
    std::cout << "Max: ";
    //bounds[1].print();

    //TODO add print function
}

/**
* @return material of the box
*/
std::shared_ptr<Material> Box::get_material() {
    return material_;
}

/**
* @param material is given to box
*/
void Box::set_material(std::shared_ptr<Material> const &material) {
    this->material_ = material;
}

/**
 * Changes the position of the Sphere with a given glm::vec3
 * @param position
 */
void Box::translate(glm::vec3 const &position) {
    for (int i = 0; i < bounds_.size(); ++i) {
        bounds_[i] += position_;
    }
    this->position_ += position_;
}

/**
 * function to return 1 or 0 for the given comparison
 * for more information visit scratchapixel + box intersection method
 * @param vec3 position
 * @param pos in the vector
 * @return 0 or 1
 */
int Box::sign(glm::vec3 const &vec3, int pos) const {
    return ((pos == 0 && vec3[0] < 0) || (pos == 1 && vec3[1] < 0) || (pos == 2 && vec3[2] < 0));
}


