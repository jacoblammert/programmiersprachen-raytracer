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
    glm::normalize(direction);
}
 
 
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
 Ray Camera::generateRay(int x, int y) const {

     glm::vec3 right = glm::cross(direction,upVector);// from the left of the camera plane to the right
     glm::vec3 top = glm::cross(direction,right); // from bottom of the camera plane to the top
     glm::normalize(right);// the vectors must be normalized
     glm::normalize(top);  // the vectors must be normalized

     float wToH = ((float) (width - 1) / (float) (height - 1));  // width to height ratio

     float xpercentage = (float) x / (float) (width - 1); // could be simplified to save two variables
     float ypercentage = (float) y / (float) (height - 1);// could be simplified to save two variables

     float scalex = (xpercentage - 0.5f) * 2 * wToH; // now a range from -1 to 1 depending on the x to width ratio
     float scaley = (ypercentage - 0.5f) * 2; // now a range from -1 to 1 depending on the y to Height ratio


     glm::vec3 vright = right * scalex; // horizontal Vector
     glm::vec3 vtop = top * scaley;     // vertical Vector

     return {position, direction + vtop + vright}; // new custom camera ray for the given pixel
 }

 /**
  * Giving the camera a position to point at, we can change the direction
  * @param pos to look at
  */
 void Camera::lookAt(glm::vec3 const& pos) {
     direction = pos - position;
     glm::normalize(direction);
 }
 
 
 void Camera::print() {
     std::cout << "Camera position: ";
     //position.print();
     std::cout << "Camera direction: ";
     //direction.print();
 
     std::cout << "Width: " << width << std::endl;
     std::cout << "Height: " << height << std::endl;
 }


