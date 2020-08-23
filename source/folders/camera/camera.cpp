//
// Created by Jacob Lammert on 17.08.2020.
//

#include "camera.hpp"

/**
 * Camera constructor without angle / fov
 * @param position of the camera as vec3
 * @param direction of the camera as vec3
 * @param width of the screen in pixel/ int
 * @param height of the screen in pixel/ int
 * @param distance of the cameraplane from the cameraorigin to the cameraplane
 */
 Camera::Camera(glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance) :
    position {position},
    direction {direction},
    width {width},
    height {height},
    distance {distance}
{
    this->direction = glm::normalize(this->direction);
}

  //Camera::Camera(glm::vec3 const& position, int width, int height, float angle, std::string const& name); - TODO
 
 
 /**
  * Generates a ray from the cameras position onto the camera plane. If the fov should be changed, the distance of the Camera plane needs to change.
  * The x and y positions are in pixelspace, like the width and the height of the camera
  *
  * //TODO this function does only work, if a distance from the cameraplane is given. If we want to use the angle, we need to use the angle to calculate the distance first
  *
  * @param x pos of the Pixel on the screen 0 to width
  * @param y pos of the Pixel on the screen 0 to height
  * @return a new Ray with the pos of the Camera and the Direction onto the camera plane in worldspace
  */
 Ray Camera::generate_ray(int x, int y) const {


     glm::vec3 right = glm::normalize(glm::cross(this->direction,upVector));// from the left of the camera plane to the right (normalized)
     glm::vec3 top = glm::normalize(glm::cross(this->direction,right)); // from bottom of the camera plane to the top (normalized)

     //float wToH = ((float) this->width / (float) this->height);  // width to height ratio

     float xpercentage = (float) x / (float) this->width; // could be simplified to save two variables
     float ypercentage = (float) y / (float) this->height;// could be simplified to save two variables

     float scalex = -(xpercentage - 0.5f) * 2 * ((float) this->width / (float) this->height); // now a range from -1 to 1 depending on the x to width ratio
     float scaley = -(ypercentage - 0.5f) * 2; // now a range from -1 to 1 depending on the y to Height ratio

     return {this->position, this->direction * this->distance + (top * scaley) + (right * scalex)}; // new custom camera ray for the given pixel
 }

 /**
  * Giving the camera a position to point at, we can change the direction
  * @param pos to look at
  */
 void Camera::lookAt(glm::vec3 const& pos) {
     this->direction = pos - this->position;
     this->direction = glm::normalize(this->direction);
 }
 
 
 void Camera::print() const{
     std::cout << "Camera position: x: " << this->position[0] << " y: " << this->position[1] << " z: " << this->position[2]<< std::endl;
     std::cout << "Camera direction: x: " << this->direction[0] << " y: " << this->direction[1] << " z: " << this->direction[2]<< std::endl;
     //direction.print();
 
     //std::cout << "Width: " << width << std::endl;
     //std::cout << "Height: " << height << std::endl;
 }

 /**
  * Sets the position of the camera to a new vector
  * @param pos new position of the camera
  */
void Camera::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void Camera::translate(glm::vec3 pos) {
    this->position += pos;
}

void Camera::move(Window const & window) {

    //std::cout<< "W: " <<window.get_key(87)<< std::endl; // W
    //std::cout<< "S: " <<window.get_key(83)<< std::endl; // S
    //std::cout<< "A: " <<window.get_key(65)<< std::endl; // A
    //std::cout<< "D: " <<window.get_key(68)<< std::endl; // D
    //std::cout<< "Space: " <<window.get_key(32)<< std::endl; // Space
    //std::cout<< "Shift: " <<window.get_key(340)<< std::endl; // Shift

    int w = window.get_key(87);
    int s = window.get_key(83);
    int a = window.get_key(65);
    int d = window.get_key(68);
    int sp = window.get_key(32);
    int sh = window.get_key(340);


    float x = w - s;
    float y = a - d;
    float z = sp - sh;



    glm::vec3 dir = glm::normalize(glm::vec3{direction[0],direction[1],0});
    glm::vec3 dir_orthogonal = glm::normalize(glm::cross(dir,this->upVector));

    dir *= x;
    dir_orthogonal *= y;

    position += glm::vec3{0,0,z} + dir + dir_orthogonal;


}

void Camera::set_direction(Window const & window) {

    glm::vec2 mouse = window.mouse_position();
    if ( 0 < mouse[0] && mouse[0] < window.window_size()[0] && 0 < mouse[1] && mouse[1] < window.window_size()[1]) {


        float mouseX = -mouse[0] / window.window_size()[0];
        float mouseY = mouse[1] / window.window_size()[1];

        float x = sin(3.14f * 2 * (mouseX));
        float y = cos(3.14f * 2 * (mouseX));
        float z = cos(3.14f * (mouseY));

        x *= (1 - abs(z));
        y *= (1 - abs(z));

        this->direction = { x, y, z};
    }
}


