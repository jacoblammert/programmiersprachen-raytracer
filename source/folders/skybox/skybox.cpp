//
// Created by Jacob Lammert on 01.09.2020.
//

#include "skybox.hpp"

Skybox::Skybox() {

    top_=    Renderer{width_,height_,"../../source/folders/skybox/posy.ppm"}; // "../../source/folders/sdfFiles/start.sdf"
    bottom_= Renderer{width_,height_,"../../source/folders/skybox/negy.ppm"}; // "../../source/folders/skybox/"
    left_=   Renderer{width_,height_,"../../source/folders/skybox/negx.ppm"};
    right_=  Renderer{width_,height_,"../../source/folders/skybox/posx.ppm"};
    front_=  Renderer{width_,height_,"../../source/folders/skybox/negz.ppm"};
    back_=   Renderer{width_,height_,"../../source/folders/skybox/posz.ppm"};
}

/**
 * Should return the color of the Skybox, does not work, because the files are not imported correctly
 * @param direction as vec3 of the ray
 * @return Color
 */
glm::vec3 Skybox::get_color(glm::vec3 direction) const {
    float x = 0, y = 0;


    Box box{{-1,-1,-1},{1,1,1}};
    glm::vec3 hit_normal;
    glm::vec3 hit_point;
    float dist = INFINITY;

    box.get_intersect_vec({{},direction},hit_point,hit_normal,dist);

    glm::vec3 color;

    if (hit_normal[0] == 1) { // left
        x = (1 + direction[1]) * 0.5f;
        y = (1 - direction[2]) * 0.5f;
        color = get_pixel_interpolated(x, y,left_);


    } else if (hit_normal[0] == -1) { // right

        x = (1 - direction[1]) * 0.5f;
        y = (1 - direction[2]) * 0.5f;
        color = get_pixel_interpolated(x, y,right_);


    } else if (hit_normal[1] == 1) { // back

        x = (1 - direction[0]) * 0.5f;
        y = (1 - direction[2]) * 0.5f;
        color = get_pixel_interpolated(x, y,back_);

    } else if (hit_normal[1] == -1) { // front

        x = (1 + direction[0]) * 0.5f;
        y = (1 - direction[2]) * 0.5f;
        color = get_pixel_interpolated(x, y,front_);

    } else if (hit_normal[2] == 1) { // Top

        x = (1 - direction[0]) * 0.5f;
        y = (1 + direction[1]) * 0.5f;
        color = get_pixel_interpolated(x, y,top_);

    } else { // Bottom

        x = (1 + -direction[0]) * 0.5f;
        y = (1 + -direction[1]) * 0.5f;
        color = get_pixel_interpolated(x, y,bottom_);
    }
    //color = (hit_normal + glm::vec3 {1,1,1}) * 0.5f;
    return color;
}


/**
 * Interpolates the Color at a given point
 * @param x position
 * @param y position
 * @param renderer Image (top, bottom, left, right, front or back)
 * @return color
 */
glm::vec3 Skybox::get_pixel_interpolated(float x, float y,Renderer const & renderer) const{

    if (0 < x && 0 < y) {
        x = x * (float) width_;
        y = y * (float) height_;

        float percentageX = x-floor(x);
        float percentageY = y-floor(y);


        x = ((int) floor(x)) % (int) width_;
        y = ((int) floor(y)) % (int) height_;


        int xMax = (x+1) < width_ ?  x+1 :x;
        int yMax = (y+1) < height_ ? y+1 :y;

        glm::vec3 middle = get_pixel(x,y,renderer);//image[y][x];
        glm::vec3 right = get_pixel(xMax,y,renderer);//image[y][xMax];
        glm::vec3 bottom = get_pixel(x,yMax,renderer);//image[yMax][x];
        glm::vec3 bottomright = get_pixel(xMax,yMax,renderer);//image[yMax][xMax];

        //if (middle[0] != 0) {
        //    std::cout << middle[0] << std::endl;
        //}

        return interpolate(interpolate(interpolate(middle,right,percentageX),interpolate(bottom,bottomright,percentageX),percentageY),
                           interpolate(interpolate(middle,bottom,percentageY),interpolate(right,bottomright,percentageY),percentageX),0.5f);
    } else {
        return {0, 0, 0};
    }
}

/**
 * get the Pixelcolor of a ppm at a given position
 * @param x position
 * @param y position
 * @param renderer Image (top, bottom, left, right, front or back)
 * @return Color
 */
glm::vec3 Skybox::get_pixel(int x,int y, const Renderer &renderer) const{

    int position = (int) (x + width_ * y);

    Color color = renderer.color_buffer()[position]; /// ppm data
    //std::cout<<"red: "<<color.r<<"green: "<<color.g<<"blue: "<<color.b<<std::endl;

    return {color.r,color.g,color.b};
}
/**
 * Interpolates two vec3 by a given amount
 * @param A vec3 No.1
 * @param B vec3 No.2
 * @param value of Interpolation
 * @return linearly interpolated vec3
 */
glm::vec3 Skybox::interpolate(const glm::vec3& A,const glm::vec3&  B, float value) const{
    return (B-A) * value + A;
}

void Skybox::set_images(Renderer renderer) {

    top_=    renderer; // "../../source/folders/sdfFiles/start.sdf"
    bottom_= renderer; // "../../source/folders/skybox/"
    left_=   renderer;
    right_=  renderer;
    front_=  renderer;
    back_=   renderer;
}
