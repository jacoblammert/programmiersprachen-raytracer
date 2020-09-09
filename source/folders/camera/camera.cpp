#include "camera.hpp"

/**
 Camera constructor with angle/ fov
 @param name of the camera
 @param fov field of view in degrees
*/

Camera::Camera (std::string const& name, float fov_x) :
    name_ {name},
    position_ {0.0f, 0.0f, 0.0f},
    direction_ {0.0000001f, 0.00000001f, -1.0f},
    width_ {1},
    height_ {1}, //TODO Berechnen durch fov in y Richtung?
    distance_ {fov_x},
    fov_x_ {fov_x}
{
    distance_ *= MATH_PI / 360.0f;
    distance_ = (float) (0.5f / std::tan(distance_)); // from presentation
}

/**
 Camera constructor with angle/ fov + position + direction + up-vector of camera
 @param name of the camera
 @param fov field of view in degrees
 @param eye position of the camera as vec3
 @param direction of the camera as vec3
 @param up up-vector of the camera as vec3
 */

Camera::Camera (std::string const& name, float fov_x, glm::vec3 const& eye, glm::vec3 const& direction, glm::vec3 const& up) :
    name_ {name},
    position_ {eye},
    direction_ {direction},
    width_{1},
    height_{1}, //TODO Berechnen durch fov in y Richtung?
    distance_{fov_x},
    up_vector_ {up},
    fov_x_ {fov_x}
{
    distance_ *= MATH_PI / 360.0f;
    distance_ = (float) (0.5f / std::tan(distance_)); // from presentation
}

/**
 * Camera constructor without angle / fov
 * @param position of the camera as vec3
 * @param direction of the camera as vec3
 * @param width of the screen in pixel/ int
 * @param height of the screen in pixel/ int
 * @param distance of the cameraplane from the cameraorigin to the cameraplane
 */

Camera::Camera (glm::vec3 const &position, glm::vec3 const &direction, int width, int height, float distance) :
    position_ {position},
    direction_ {direction},
    width_ {width},
    height_ {height},
    distance_ {distance}
{
    this->direction_ = glm::normalize(this->direction_);
}

/**
 *
 * Camera constructor with fov - looks at 0, 0, -1
 * @param position of the camera as vec3
 * @param direction of the camera as vec3
 * @param width of the screen in pixel/ int
 * @param height of the screen in pixel/ int
 * @param fov field of view in degrees
 */

Camera::Camera (glm::vec3 const &position, int width, int height, float fov_x) :
    position_ {position},
    direction_ {0.0f, 0.0f, -1.0f},
    width_ {width},
    height_ {height},
    distance_ {fov_x}
{
    distance_ *= MATH_PI / 360.0f;
    distance_ = (float) (0.5f / std::tan(distance_)); // from presentation
    
}


/**
 * Generates a ray from the cameras position onto the camera plane. If the fov should be changed, the distance of the camera plane needs to change.
 * The x and y positions are in pixelspace, like the width and the height of the camera
 *
 * @param x pos of the Pixel on the screen 0 to width
 * @param y pos of the Pixel on the screen 0 to height
 * @return a new Ray with the pos of the Camera and the direction onto the camera plane in worldspace
 */

Ray Camera::generate_ray (int x, int y) const {
    
    // from the left of the camera plane to the right (normalized)
    glm::vec3 right = glm::normalize (glm::cross(direction_, up_vector_));
    // from bottom of the camera plane to the top (normalized)
    glm::vec3 top = glm::normalize (glm::cross(direction_, right));
    
    // TODO could be simplified to save two variables
    float x_percentage = (float) x / (float) width_;
    float y_percentage = (float) y / (float) height_;

    // now a range from -1 to 1 depending on the x to width ratio
    float scale_x = -(x_percentage - 0.5f) * 2 * ((float) width_ / (float) height_);
    // now a range from -1 to 1 depending on the y to height ratio
    float scale_y = -(y_percentage - 0.5f) * 2;
    
    
    // makes image blurry - like looking through the rong glasses / uneven glass
    float a = random_float();
    float b = random_float();
    float c = random_float();
    // vector in random direction
    glm::vec3 blur = glm::normalize (glm::vec3 {a, b, c});
    
    // depth of field
    // "strength of depth effect" - standard 0
    blur *= dof_strength_;
    
    glm::vec3 direction = glm::normalize((direction_ * distance_) + (top * scale_y) + (right * scale_x));


    /// The focal point will be set to a given distance, therefore there must be an offset in position and direction which cancel at the direction
    direction *= focal_length_;
    
    // new custom camera ray for the given pixel
    return {- blur + position_, direction + blur};
}

/**
 * Sets the position of the camera to a new vector
 * @param pos new position of the camera
 */

void Camera::set_position (glm::vec3 const& pos) {
    position_ = pos;
}

/**
 Sets the width and height of the screen
 @param width of the screen in pixel/ int
 @param height of the screen in pixel/ int
*/

void Camera::set_width_height (int width, int height) {
    width_ = width;
    height_ = height;
}

/**
 Sets the direction in which the camera faces - camera can be moved by changing mouse position
 @param window of scene
*/

void Camera::set_direction (Window const &window) {
    
    glm::vec2 mouse = window.mouse_position();
    
    // looks if mouse is in window
    if (0 < mouse[0] && mouse[0] < window.window_size()[0] && 0 < mouse[1] && mouse[1] < window.window_size()[1]) {
        
        float mouse_x = -mouse[0] / window.window_size()[0];
        float mouse_y = mouse[1] / window.window_size()[1];

        // x, y and z are calculated by the position of the mouse (moving mose to the left side = direction goes to a position on the "left" side)
        float x = sin (3.14f * 2 * mouse_x); // sin -> rotation around origin, 3.14f * 2 * mouse_x for a range from 0 to 2 pi
        float y = cos (3.14f * 2 * mouse_x); // cos -> rotation around origin, 3.14f * 2 * mouse_y for a range from 0 to 2 pi
        float z = cos (3.14f * mouse_y);
        
        // Need to be scaled because we dont have a cylinder, but a sphere (z = 1/ z = 0) => x or y must be close to zero and not 1 as largest possible value
        x *= (1 - abs(z));
        // Need to be scaled because we dont have a cylinder, but a sphere (z = 1/ z = 0) => x or y must be close to zero and not 1 as largest possible value
        y *= (1 - abs(z));
        
        direction_ = glm::normalize(glm::vec3 {x, z, -y});
    }
}


/**
 Sets the depth of field by getting the strength of the depth effect and the length of the focal area
 @param dof_strength  strength of depth effect - standard 0
 @param focal_length legth of focal area
*/

void Camera::set_depth_of_field (float dof_strength, float focal_length) {

    // The focal point will be set to a given distance, therefore there must be an offset in position and direction which cancel at the direction
    dof_strength_ = focal_length < INFINITY ? (focal_length / 250) : 0.1f;
    focal_length_ = focal_length < INFINITY ? focal_length : 4.0f;
    
}

/**
 Gives the position of the camera
*/

glm::vec3 Camera::get_position() const {
    return position_;
}

/**
 Gives the name of the camera
*/

std::string Camera::get_name() const {
    return name_;
}

/**
 Gives informations of the camera (name, position, ..) as a string - for creating the .sdf
*/

std::string Camera::get_information() const {
    std::string information = " " + name_ + " " + std::to_string(fov_x_) + " " + std::to_string(position_[0]) + " " + std::to_string(position_[1]) + " " + std::to_string(position_[2]) + " " + std::to_string(direction_[0]) + " " + std::to_string(direction_[1]) + " " + std::to_string(direction_[2]) + " " + std::to_string(up_vector_[0]) + " " + std::to_string(up_vector_[1]) + " " + std::to_string(up_vector_[2]) + "\n";
    return information;
}

/**
 Prints the position and direction of the camera
*/

void Camera::print() const {
    std::cout << "Camera position: x: " << this->position_[0] << " y: " << this->position_[1] << " z: " << this->position_[2] << std::endl;
    std::cout << "Camera direction: x: " << this->direction_[0] << " y: " << this->direction_[1] << " z: " << this->direction_[2] << std::endl;
}



/**
 Translates the camera by using given position
 @param pos vector the camera is translated by
*/

void Camera::translate (glm::vec3 const &pos) {
    // translating the camera by pos
    position_ += pos;
}

/**
 * Giving the camera a position to point at - we can change the direction
 * @param pos position to look at
 */

void Camera::look_at (glm::vec3 const &pos) {
    direction_ = pos - position_;
    direction_ = glm::normalize (direction_);
}

/**
 Camera can be moved by using wasd, space and shift
 @param window of the scene
*/
void Camera::move (Window const &window) {
    
    int w = window.get_key(87);
    int s = window.get_key(83);
    int a = window.get_key(65);
    int d = window.get_key(68);
    int space = window.get_key(32);
    int shift = window.get_key(340);
    
    // w: forwards movement
    // a: left movement
    // s: backwards movement
    // d: right movement
    // space: up movement
    // shift: down movement
    float x = w - s;
    float z = a - d;
    float y = space - shift;
    
    // Calculating the direction the camera moves in with when one of the w a s d Keys are pressed (ws in one direction (dir) and dir_orthogonal for a and d)
    glm::vec3 dir = glm::normalize (glm::vec3 {direction_[0], 0, direction_[2]});
    glm::vec3 dir_orthogonal = glm::normalize (glm::cross (dir, up_vector_));
    
    dir *= x;
    dir_orthogonal *= z;
    // New position calculated
    position_ += glm::vec3 {0, y, 0} + dir + dir_orthogonal;
    
}

/**
 * for roughness - get a value between -1 and 1
 * @return random float value
 */
float Camera::random_float() const {
    // range from -range to range
    return ((((float) rand()) / (float) (RAND_MAX / 2.0f)) - 1.0f);
}
