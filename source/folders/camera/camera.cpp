//
// Created by Jacob Lammert on 17.08.2020.
//

#include "camera.hpp"
 
 Camera::Camera(glm::vec3 const& position, glm::vec3 const& direction, int width, int height, float distance) :
    position {position},
    direction {direction},
    width {width},
    height {height},
    distance {distance}
{
    //distance = 
}
 
 
 /**
  * Generates a ray from the cameras position onto the camera plane. If the fov should be changed, the distance of the Camera plane needs to change.
  * The x and y positions are in pixelspace, like the width and the height of the camera
  * @param x pos of the Pixel on the screen 0 to width
  * @param y pos of the Pixel on the screen 0 to height
  * @return a new Ray with the pos of the Camera and the Direction onto the camera plane in worldspace
  */
 void Camera::generateRay(int x, int y, Ray const& ray) const {
     //ray.position = position; TODO overload =
     //ray.direction = glm::vec3 (x, y, -distance) - position;
 }

 
 void Camera::lookAt(glm::vec3 const& pos) {
     direction = pos - position;
     glm::normalize(direction);
     //direction.print();
     //update();
 }
 
 
 void Camera::print() {
     std::cout << "Camera position: ";
     //position.print();
     std::cout << "Camera direction: ";
     //direction.print();
 
     std::cout << "Width: " << width << std::endl;
     std::cout << "Height: " << height << std::endl;
 }


