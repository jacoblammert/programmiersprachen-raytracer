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
Box::Box(std::string const &name, const glm::vec3 &min_x_y_z, const glm::vec3 &max_x_y_z) {
    shape_type_ = BOX;
    name_ = name;
    position_ = (min_x_y_z + max_x_y_z) * 0.5f;
    // bounds[0] = min of box, bounds[1] = max of box
    bounds_.push_back(min_x_y_z);
    bounds_.push_back(max_x_y_z);
}

/**
* box constructor for axis alligned box with a min and max vector for example: {-1,-1,-1} {1,1,1}
* @param name name of box
* @param minXminYminZ minimum of the box (individual values must be smaller than their counterpart in max vector)
* @param maxXmaxYmaxZ maximum of the box (individual values must be greater than their counterpart in min vector)
*/
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
Box::Box(glm::vec3 const &pos, float xScale, float yScale, float zScale) {
    position_ = pos;

    glm::vec3 min_x_y_z = {pos[0] - (xScale / 2), pos[1] - (yScale / 2), pos[2] - (zScale / 2)};
    glm::vec3 max_x_y_z = {pos[0] + (xScale / 2), pos[1] + (yScale / 2), pos[2] + (zScale / 2)};
    bounds_.push_back(min_x_y_z);
    bounds_.push_back(max_x_y_z);
}

/**
 * function to return 1 or 0 for the given comparison
 * for more information visit scratchapixel + box intersection method
 * /// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
 * @param vec3 position
 * @param pos in the vector
 * @return 0 or 1
 */
int Box::sign(glm::vec3 const &vec3, int pos) const {
    return ((pos == 0 && vec3[0] < 0) || (pos == 1 && vec3[1] < 0) || (pos == 2 && vec3[2] < 0));
}

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

    glm::vec3 ray_position = ray.position;
    glm::vec3 ray_direction = ray.direction;

    ray_position = ray_position - position_;
    ray_position = rotation_matrix_ * ray_position;
    ray_position = ray_position + position_;

    ray_direction = rotation_matrix_ * ray_direction;
    ray_direction = glm::normalize(ray_direction);
    
    glm::vec3 ray_direction_rotated = ray_direction;
    ray_direction = glm::vec3 {1/ray_direction[0], 1/ray_direction[1], 1/ray_direction[2]};

    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;

    // look where the ray hits the ray hits the "x-plane" of the maximum and minimum bound of the box
    t_x_min = (bounds_[sign(ray_direction, 0)][0] - ray_position[0]) * ray_direction[0];
    t_x_max = (bounds_[1 - sign(ray_direction, 0)][0] - ray_position[0]) * ray_direction[0];

    // look where the ray hits the ray hits the "y-plane" of the maximum and minimum bound of the box
    t_y_min = (bounds_[sign(ray_direction, 1)][1] - ray_position[1]) * ray_direction[1];
    t_y_max = (bounds_[1 - sign(ray_direction, 1)][1] - ray_position[1]) * ray_direction[1];
    
    // look where the ray hits the ray hits the "y-plane" of the maximum and minimum bound of the box
    t_z_min = (bounds_[sign(ray_direction, 2)][2] - ray_position[2]) * ray_direction[2];
    t_z_max = (bounds_[1 - sign(ray_direction, 2)][2] - ray_position[2]) * ray_direction[2];

    
    // ray misses box
    if ((t_x_min > t_y_max) || (t_y_min > t_x_max))
        return false;
    
    // t_x_min and t_x_max are set to the points where the t value is the smallest
    if (t_y_min > t_x_min)
        t_x_min = t_y_min;
    
    if (t_y_max < t_x_max)
        t_x_max = t_y_max;

    // ray misses box
    if ((t_x_min > t_z_max) || (t_z_min > t_x_max))
        return false;
    
    // t_x_min and t_x_max are set to the points where the t value is the smallest
    if (t_z_min > t_x_min)
        t_x_min = t_z_min;
    
    if (t_z_max < t_x_max)
        t_x_max = t_z_max;


    if (0 < t_x_min && t_x_min < distance) {
        ray_direction_rotated *= t_x_min;
        // save information about the intersection
        distance = t_x_min;
        hit_point = ray_position + ray_direction_rotated;
        hit_normal = get_normal(hit_point - position_);
        hit_point -= position_;
        hit_point = rotation_matrix_inverse * hit_point;
        hit_point += position_;

        hit_normal = rotation_matrix_inverse * hit_normal;

        return true;
    }
    if (t_x_min < 0 && 0 < t_x_max && t_x_max < distance) {
        ray_direction_rotated *= t_x_max;
        // save information about the intersection
        hit_point = ray_position + ray_direction_rotated;
        hit_normal = get_normal(hit_point - position_);
        hit_point -= position_;
        hit_point = rotation_matrix_inverse * hit_point;
        hit_point += position_;

        distance = glm::length(ray.position - hit_point);

        hit_normal = rotation_matrix_inverse * hit_normal;
        return true;
    }

    return false;
}

/**
 * Only important for the Skybox
 * now the ray hits the Skybox, therfore we can remove a few things
 * @param direction
 * @param hit_normal
 */
void Box::get_intersect_vec(glm::vec3 &direction, glm::vec3 &hit_normal) const {

    glm::vec3 ray_direction = direction;

    ray_direction = 1.0f / direction;

    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;

    t_x_min = (bounds_[sign(ray_direction, 0)][0]) * ray_direction[0];
    t_x_max = (bounds_[1 - sign(ray_direction, 0)][0]) * ray_direction[0];

    t_y_min = (bounds_[sign(ray_direction, 1)][1]) * ray_direction[1];
    t_y_max = (bounds_[1 - sign(ray_direction, 1)][1]) * ray_direction[1];

    // t_x_min and t_x_max are set to the points where the t value is the smallest
    if (t_y_min > t_x_min)
        t_x_min = t_y_min;
    if (t_y_max < t_x_max)
        t_x_max = t_y_max;

    t_z_min = (bounds_[sign(ray_direction, 2)][2]) * ray_direction[2];
    t_z_max = (bounds_[1 - sign(ray_direction, 2)][2]) * ray_direction[2];

    // t_x_min and t_x_max are set to the points where the t value is the smallest
    if (t_z_min > t_x_min)
        t_x_min = t_z_min;
    if (t_z_max < t_x_max)
        t_x_max = t_z_max;


    if (0 < t_x_min) {
        direction *= t_x_min;
        // get normal where object is hit
        hit_normal = get_normal(direction);
    }
    if (t_x_min < 0 && 0 < t_x_max) {
        direction *= t_x_max;
        // get normal where object is hit
        hit_normal = get_normal(direction);
    }
}


/**
 * Important for BoundingBox intersection testing. Similar to the previous method, but without all the useless stuff (could be faster)
 * @param ray that gets tested against this box
 * @return true if the ray hits
 */
bool Box::get_intersect(const Ray &ray) const {


    if (bounds_[0][0] < ray.position[0] && ray.position[0] < bounds_[1][0] &&
        bounds_[0][1] < ray.position[1] && ray.position[1] < bounds_[1][1] &&
        bounds_[0][2] < ray.position[2] && ray.position[2] < bounds_[1][2]) {
        // The position of the ray is inside of the box, therefore it intersects the box
        return true;
    }

    glm::vec3 ray_direction = {1 / ray.direction[0], 1 / ray.direction[1], 1 / ray.direction[2]};

    float t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max;

    t_x_min = (bounds_[sign(ray_direction, 0)][0] - ray.position[0]) * ray_direction[0];
    t_x_max = (bounds_[1 - sign(ray_direction, 0)][0] - ray.position[0]) * ray_direction[0];

    t_y_min = (bounds_[sign(ray_direction, 1)][1] - ray.position[1]) * ray_direction[1];
    t_y_max = (bounds_[1 - sign(ray_direction, 1)][1] - ray.position[1]) * ray_direction[1];

    if ((t_x_min > t_y_max) || (t_y_min > t_x_max))
        return false;


    if (t_y_min > t_x_min)
        t_x_min = t_y_min;


    if (t_y_max < t_x_max)
        t_x_max = t_y_max;


    t_z_min = (bounds_[sign(ray_direction, 2)][2] - ray.position[2]) * ray_direction[2];
    t_z_max = (bounds_[1 - sign(ray_direction, 2)][2] - ray.position[2]) * ray_direction[2];

    if ((t_x_min > t_z_max) || (t_z_min > t_x_max))
        return false;
    if (t_z_min > t_x_min)
        t_x_min = t_z_min;
    if (t_z_max < t_x_max)
        t_x_max = t_z_max;

    return (t_x_min < 0 && 0 < t_x_max) || 0 < t_x_min;
}

/**
 * returns the normal for a given position
 * @param position position (Hitposition)
 * @return normal at the given position. Either {1,0,0},{0,1,0},{0,0,1} or {-1,0,0},{0,-1,0},{0,0,-1}
 */
glm::vec3 Box::get_normal(glm::vec3 const &pos) const {

    glm::vec3 size = get_max() - get_min();

    size = pos / size;

    // X is largest
    if (fabs(size[0]) > fabs(size[1]) && fabs(size[0]) > fabs(size[2])) {
        return glm::vec3{1, 0, 0} * glm::sign(size[0]);
    // Y is largest
    } else if (fabs(size[1]) > fabs(size[2])) {
        return glm::vec3{0, 1, 0} * glm::sign(size[1]);
    // Z is largest
    } else {
        return glm::vec3{0, 0, 1} * glm::sign(size[2]);
    }
}

/**
 * @return a vector with the minimal values of x, y and z of the box
 */
glm::vec3 Box::get_min() const {
    glm::vec3 diagonal = bounds_[1] - bounds_[0];
    diagonal = glm::vec3{0.5, 0.5, 0.5} * glm::length(diagonal);
    return position_ -
           diagonal; /// like a sphere with the radius of the distance to the origin of the point the furthest away from the center
}


/**
 * @return a vector with the maximal values of x, y and z of the box
 */
glm::vec3 Box::get_max() const {
    glm::vec3 diagonal = bounds_[1] - bounds_[0];
    diagonal = glm::vec3{0.5, 0.5, 0.5} * glm::length(diagonal);
    return position_ +
           diagonal; /// like a sphere with the radius of the distance to the origin of the point the furthest away from the center
}

/**
 * @return the position of the box
 */
glm::vec3 Box::get_median() const {
    return position_;
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
        bounds_[i] += position;
    }
    this->position_ += position;
}

// TODO noch nötig?

/**
 * outputs important information in the console
 */
void Box::print(std::fstream &file) const {
    //std::string information = std::to_string(bounds_[0][0]) + " " + std::to_string(bounds_[0][1]) + " " + std::to_string(bounds_[0][2]) + " " + std::to_string(bounds_[1][0]) + " " + std::to_string(bounds_[1][1]) + " " + std::to_string(bounds_[1][2]);
    //file << information;
}


/**
* Gives information about the box in a string
* @return string with information
*/
std::string Box::get_information() const {
    std::string information = name_ + " "
            + std::to_string(bounds_[0][0]) + " " + std::to_string(bounds_[0][1]) + " " + std::to_string(bounds_[0][2])+ " "
            + std::to_string(bounds_[1][0]) + " " + std::to_string(bounds_[1][1]) + " " + std::to_string(bounds_[1][2]) + " "
            + std::to_string(axis_[0]) + " " + std::to_string(axis_[1]) + " " + std::to_string(axis_[2]) + " "
            + material_->name;
    return information;
}

