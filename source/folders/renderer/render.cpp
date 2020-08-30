#include "render.hpp"

#include <cmath>
#include <utility>

Render::Render() = default;


/**
 * A method to get the pure color of a point, if an Object has been hit.
 * @param ray
 * @param depth depth of the ray
 * @param lights pointer to the lightvector to calculate the lightstrength
 * @param boundingBox to get intersections with other shapes from the ray to go to all the lightsources
 * @return color consisting of brightness for that point and each light and color of the shape blended together
 */
glm::vec3 Render::get_color(Ray ray, int depth) const {



    //TODO split functions into different subfunctions to make visibility better




    /**Get Intersected Object here**/

    glm::vec3 hit_point;
    glm::vec3 hit_normal;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;// = nullptr;

    composite_->get_intersected_shape(ray, shape, hit_point, hit_normal,
                                      dist); // TODO pass pointer instead of all the parameters



    if (dist == INFINITY) { // shape has not been hit
        //TODO implement skybox or background
        return glm::vec3{0, 0, 0}; // returns black, if no shape has been hit
    }


    float glossy = shape->get_material()->glossy_;
    float opacity = shape->get_material()->opacity_;
    float roughness = shape->get_material()->roughness_;

    glm::vec3 color_final;//{shape->get_material()->color_};

    //color_final = color_final * (0.03f * (1 - transparency));// ?


    color_final = get_brightness_color(ray, hit_point, hit_normal, depth, shape);


    glm::vec3 reflection_color;
    glm::vec3 refraction_color;

    if (depth < 10) { // calculates a maximum of n reflections because depth < n

        /*** Color calculations reflection: //TODO clean up in different function ***/
        if (glossy > 0) {
            int reflection_samples = roughness == 0.0f ? 1 : 1 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < reflection_samples; ++i) {
                reflection_color += get_reflected_color(ray, hit_point, hit_normal, depth,
                                                        shape);// increasing the depth is not really necessary
            }
            reflection_color *= glossy / (float) reflection_samples;
        }


        /*** Colorcalculations refractions: //TODO clean up in different function ***/
        if (opacity > 0) {
            int refraction_samples = roughness == 0.0f ? 1 : 1 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < refraction_samples; ++i) {
                refraction_color += get_refracted_color(ray, hit_point, hit_normal, depth,
                                                        shape);// increasing the depth is not really necessary
            }
            refraction_color *= opacity / (float) refraction_samples;
        }
    }


    /*** Color mixing: //TODO clean up in different function (Optional) ***/
    color_final *= (1.0f - opacity);
    color_final *= (1.0f - glossy);

    color_final += reflection_color + refraction_color;

    return color_final / glm::length(glm::vec3{1 - opacity, opacity, glossy});
}

/**
 * Set the composite object for this Renderer
 * All shapes inside the composite object will be drawn.
 * @param composite object with the information to be rendered
 */
void Render::set_composite(std::shared_ptr<Composite> const &composite_1) {
    this->composite_ = composite_1;
}

void Render::set_lights(std::vector<std::shared_ptr<Light>> const &lights_1) {
    this->lights_ = lights_1;
}


/**
 * Function to calculate the color of the reflected ray
 * @param ray will be reflected and sent away
 * @param hit_point Point where the old ray has intersected the shape
 * @param hit_normal normal at the intersected position
 * @param depth of the ray to set a recursion depth
 * @param shape pointer of the last intersected shape (for material/ refractive index information later on)
 * @return the calculated color after all recursions came to an end
 */
glm::vec3
Render::get_reflected_color(Ray const &ray, glm::vec3 const &hit_point, glm::vec3 const &hit_normal, int depth,
                            std::shared_ptr<Shape> const &shape) const {

    glm::vec3 reflected_vector = get_reflected_vec3(ray.direction_, hit_normal);

    if (shape->get_material()->roughness_ != 0.0) { // no roughness = no multiple rays to get a mat material effect
        glm::vec3 offset = {0, 0, 0};

        // the first vector does not need to be changed, but I did it this way for now

        for (int i = 0; i < 3; ++i) {
            offset[i] = random_float();
        }
        offset = glm::normalize(offset) * shape->get_material()->roughness_;
        reflected_vector = reflected_vector + offset;
    }


    Ray reflection_ray = {hit_point + hit_normal * 0.0001f, reflected_vector};

    return get_color(reflection_ray, depth);
}

/**
 * Function to calculate the color of the refracted ray
 * @param ray stores information about the refractive index of the new/ old object to make the correct calculations
 * the ray does have a refractive index of 1.0 in the beginning regardless of its actual position -> wrong calculations when
 * the camera starts inside an object/ two shapes intersect
 *
 * It works for chromatic aberration as well, but is really expensive
 * @param hit_point Point where the old ray has intersected the shape
 * @param hit_normal normal at the intersected position
 * @param depth of the ray to set a recursion depth
 * @param shape pointer of the last intersected shape (for material/ refractive index information)
 * @return the calculated color after all recursions came to an end
 */
glm::vec3
Render::get_refracted_color(Ray const &ray, glm::vec3 const &hit_point, glm::vec3 const &hit_normal, int depth,
                            std::shared_ptr<Shape> const &shape) const {
    glm::vec3 total_color{};

    int max = 3; // for red, green and blue
    int min = 0; // start

    if (depth > 1 || shape->get_material()->aberration_strength_ <= 0) {
        min = 1; // only one color value will be calculated
        max = 2; // only one color value will be calculated
    }

    for (int i = min; i < max; ++i) {

        float aberration = (float) i - 1;
        aberration *= -shape->get_material()->aberration_strength_; // scale of the aberration


        glm::vec3 position = {};
        glm::vec3 refracted_vector;

        if (depth % 2 == 0) { // Camera is in air // Normal is ok
            position = hit_point - hit_normal * 0.0001f; // small offset to not intersect the last shape

            refracted_vector = get_refracted_vec3(ray.direction_, hit_normal,
                                                  (1 / (
                                                          shape->get_material()->refractive_index_ +
                                                          aberration))); // the 1 is the refractive index of air (could be changed to any medium if we wanted)
        } else {

            position = hit_point + hit_normal * 0.0001f; // small offset to not intersect the last shape

            refracted_vector = get_refracted_vec3(ray.direction_, -hit_normal,
                                                  ((shape->get_material()->refractive_index_ + aberration) /
                                                   1)); // the 1 is the refractive index of air (could be changed to any medium if we wanted)
        }


        if (shape->get_material()->roughness_ != 0.0) {
            refracted_vector += glm::normalize(glm::vec3{random_float(), random_float(), random_float()}) *
                                shape->get_material()->roughness_;
            /// Adds a vector in a random direction with the length of the roughness
        }

        glm::vec3 color = get_color({position, refracted_vector}, depth + 1);

        total_color[i] = color[i];

        if (depth > 1 || shape->get_material()->aberration_strength_ == 0.0f) {
            /// no chromatic aberation at all
            total_color = color;
        }

    }
    return total_color;
}

/**
 * for roughness to get a value between -1 and 1
 * @return random float value
 */
float Render::random_float() const {
    return ((((float) rand()) / (float) (RAND_MAX / 2.0f)) - 1.0f);// range from -range to range
}


/**
 * Function to get a refracted Vector of a normal vector
 * @param vector input
 * @param normal normal of camera
 * @param n n of Material1/ n of Material2
 * @return new refracted vector
 */
glm::vec3 Render::get_refracted_vec3(const glm::vec3 &vector, glm::vec3 const &normal, float n) const {

    float cos_i = -glm::dot(normal, vector);
    float sin_t2 = (n * n * (1.0f - (float) (cos_i * cos_i)));
    if (sin_t2 > 1.0) return get_reflected_vec3(vector, normal);
    return vector * n + (normal * (float) (n * cos_i - sqrt(1.0 - sin_t2)));
}

/**
 * Function to get a reflected Vector of a normal
 * @param vector input to be reflected
 * @param normal vector to be reflected of
 * @return reflected vector (not normalized)
 */
glm::vec3 Render::get_reflected_vec3(const glm::vec3 &vector, glm::vec3 const &normal) const {
    return normal * (float) (2.0f * fmax(glm::dot(normal, -vector), 0)) + vector;
}

/**
 * Correct calculation for the brightness from the presentation
 * @param ray which was tested
 * @param hit_point of the hit
 * @param hit_normal of the hit
 * @param depth of the hit
 * @param shape hit (to get the material)
 * @return color
 */
glm::vec3 Render::get_brightness_color(Ray const &ray, glm::vec3 hit_point, glm::vec3 hit_normal, int depth,
                                       std::shared_ptr<Shape> const &shape) const {

    glm::vec3 intensity_color = shape->get_material()->color_ambient_ * glm::vec3{0.05f, 0.05f, 0.05f}; // color_ambient * ambient color of scene
    glm::vec3 specular;
    glm::vec3 diffuse;


    //if (!(transparency == 1.0f || glossy == 1.0f)) {
    float light_distance;
    float angle;
    glm::vec3 hit_to_light;

    for (int i = 0; i < lights_.size(); ++i) {
        //if (lights_[i]->brightness_.x > 0) { //brightness vec3!

        hit_to_light = (lights_[i]->position_ - hit_point);

        light_distance = glm::length(hit_to_light);
        hit_to_light = glm::normalize(hit_to_light);

        angle = glm::dot(hit_normal, hit_to_light);

        if (0 < angle && depth < 20) { // normal facing in direction of light

            ///Specular:
            glm::vec3 mirror_to_light = glm::normalize(get_reflected_vec3(-hit_to_light, hit_normal));
            float stength_specular = glm::dot(-ray.direction_, mirror_to_light);
            stength_specular = (float) std::pow(stength_specular, shape->get_material()->reflective_exponent_);

            specular = shape->get_material()->color_specular_ * stength_specular;


            /// Diffuse:
            diffuse = shape->get_material()->color_diffuse_ * angle + specular;

            diffuse = lights_[i]->brightness_ * diffuse;



            /**/
            //glm::vec3 light_color = (diffuse * lights_[i]->color_) / (light_distance * light_distance); // brightness


            /// New Shadow calculations
            /// No. of samples = important if range != 0

            //glm::vec3 color;
            glm::vec3 offset = glm::vec3{0,0,0};
            glm::vec3 newpos = hit_point + hit_normal * 0.0001f; // new position to shoot a ray from to not hit the last shape hit instantly

            int samples = 1;
            int count = 1;

            for (int j = 0; j < samples; ++j) {

                if (lights_[i]->hardness_ < 1.0f) {
                    offset = glm::normalize(glm::vec3{random_float(), random_float(), random_float()}) * (1.0f - lights_[i]->hardness_);
                }
                //if (depth < 10) {
                    offset += glm::normalize(hit_to_light) * 0.001f;
                //} else {
                //    offset += hit_normal;
                //}
                /// offset is now a vec3 with a random direction and the length of roughness


                glm::vec3 new_hit_point;
                glm::vec3 new_hit_normal;
                float new_dist = INFINITY;
                std::shared_ptr<Shape> new_shape;

                composite_->get_intersected_shape({newpos, offset}, new_shape, new_hit_point,new_hit_normal, new_dist);

                if (INFINITY == new_dist || light_distance < new_dist) { // light is closer than anything hit
                    //count++;
                    //color += light_color;

                    ///The original hit_point is in the light

                    intensity_color += diffuse/(light_distance * light_distance);

                }
                //if (new_dist < light_distance && depth < 4/* && roughness == 0.0f*/) { // something in the way
                //    count++;
                //    //// newpos has changed, but we want to intersect the next object to get refraction
                //    color += get_color({newpos + new_hit_normal * 0.001f , offset}, 1 + depth);
                //}

                //intensity_color += ;
            }

            //color /= count;
            //intensity_color += color;

            /**/
            //}
        }
        //}
    }
    return intensity_color;
}

