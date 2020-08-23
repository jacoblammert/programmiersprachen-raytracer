//
// Created by Jacob Lammert on 21.08.2020.
//

#include "render.hpp"

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
glm::vec3 Render::get_color(Ray const& ray, int depth) const{



    //TODO split functions into different subfunctions to make visibility better




    /**Get Intersected Object here**/

    glm::vec3 hit_point;
    glm::vec3 hit_normal;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;

    composite_->get_intersected_shape(ray, shape, hit_point, hit_normal, dist); // TODO pass pointer instead of all the parameters



    if (dist == INFINITY) { // shape has not been hit
        //TODO implement skybox or background
        return {}; // returns black, if no shape has been hit
    }


    float glossy = 0.0f;//shape->getMaterial()->getGlossy();
    float transparency = 0.00f;//shape->getMaterial()->getTransparency();
    float roughness = 0.0;//shape->getMaterial()->getRoughness();


    glm::vec3 color_final{1,1,1};//shape->getMaterial()->getColor()};

    color_final = color_final  * (0.05f * (1 - transparency));


    /*** Colorcalculations: //TODO clean up in different function ***/
    if (!(transparency == 1.0f || glossy == 1.0f)) {
        float light_strength = 0;


        glm::vec3 hit_to_light = {};
        for (int i = 0; i < lights_.size(); ++i) {

            if (lights_[i]->brightness > 0) {

                glm::vec3 normal = hit_normal;
                //Normal = glm::normalize(Normal);


                hit_to_light = (lights_[i]->position - hit_point);
                light_strength = glm::length(hit_to_light);
                hit_to_light = glm::normalize(hit_to_light);

                float angle = glm::dot(normal,hit_to_light);

                if (0 < angle) { // normal facing in direction of light
                    normal *= 0.1f;
                    if (!cast_shadow_ray({hit_point + normal, hit_to_light}, light_strength)) {

                        glm::vec3 light_color = (lights_[i]->color * (angle * lights_[i]->brightness)) / (light_strength * light_strength); // brightness

                        color_final += light_color;//(shape->getMaterial()->getColor() * Lightcolor); //TODO change in one line or with material
                    }
                }
            }
        }
    }


    glm::vec3 reflection_color;
    glm::vec3 refraction_color;

    if (depth < 20) { // calculates as n reflections because depth < n

        //glm::vec3 Normal{HitNormal};//HitNormal.scale(0.001);


        /*** Color calculations reflection: //TODO clean up in different function ***/
        if (glossy > 0) {
            int reflection_samples = roughness == 0.0f ? 1 : 3 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < reflection_samples; ++i) {
                reflection_color += get_reflected_color(ray, hit_point, hit_normal, depth, shape);// increasing the depth is not really necessary
            }
            reflection_color *= glossy / (float) reflection_samples;
        }


        /*** Colorcalculations refractions: //TODO clean up in different function ***/
        if (transparency > 0) {
            int refraction_samples = roughness == 0.0f ? 1 : 3 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < refraction_samples; ++i) {
                refraction_color += get_refracted_color(ray, hit_point, hit_normal, depth,
                                                     shape);// increasing the depth is not really necessary
            }
            refraction_color *= transparency / (float) refraction_samples;
        }
    }


    /*** Color mixing: //TODO clean up in different function (Optional) ***/
    color_final *= (1.0f - transparency);

    color_final += (reflection_color + refraction_color);

    float value = 1 - transparency;

    glm::vec3 brightness{value, transparency, glossy};

    color_final /= glm::length(brightness);

    return color_final;
}

/**
 * Set the composite object for this Renderer
 * All shapes inside the composite object will be drawn.
 * @param composite object with the information to be rendered
 */
void Render::set_composite(std::shared_ptr<Composite> const& composite_1) {
    this->composite_ = std::move(composite_1);
}

void Render::set_lights(std::vector<std::shared_ptr<Light>> const& lights_1) {
    this->lights_ = std::move(lights_1);
}

/**
 * returns false, if a ray sent in the light direction has intersected with a shape
 * @param ray to fly to the lightsource
 * @param distance to the light, if it hits anything closer, we have shadow
 * @return intersection further than distance
 */
bool Render::cast_shadow_ray(const Ray& ray, float distance) const{


    glm::vec3 hit_point;
    glm::vec3 hit_normal;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;

    composite_->get_intersected_shape(ray, shape, hit_point, hit_normal, dist);
    //TODO clean up
    return (dist != INFINITY) && dist < distance && dist >= 0;
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
glm::vec3 Render::get_reflected_color(Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape/*** dont remove, we need the roughness*/) const{

    glm::vec3 reflected_vector = get_reflected(ray.direction, hit_normal);


    float roughness = 0;//shape->getMaterial()->getRoughness();//TODO change
    if (roughness != 0.0) { // no roughness = no multiple rays to get a mat material effect
        glm::vec3 offset = {0, 0, 0};

        // the first vector doesent need to be changed, but I did it this way for now

        for (int i = 0; i < 3; ++i) {
            offset[i] = random_float(roughness);
        }
        offset = glm::normalize(offset) * roughness;
        reflected_vector = reflected_vector + offset;
    }


    Ray reflection_ray = {hit_point + hit_normal * 0.001f, reflected_vector};

    return get_color(reflection_ray, depth);
}

/**
 * Function to calculate the color of the refracted ray
 * @param ray stores information about the refractive index of the new/ old object to make the correct calculations
 * the ray does have a refractive index of 1.0 in the beginning regardless of its actual position -> wrong calculations when
 * the camera starts inside an object/ two shapes intersect
 * @param hit_point Point where the old ray has intersected the shape
 * @param hit_normal normal at the intersected position
 * @param depth of the ray to set a recursion depth
 * @param shape pointer of the last intersected shape (for material/ refractive index information)
 * @return the calculated color after all recursions came to an end
 */
glm::vec3 Render::get_refracted_color(Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth, std::shared_ptr<Shape> const& shape/*** dont remove, we need the reflective index*/) const{

    glm::vec3 position = {};

    float new_refraction_index;
    float old_refraction_index;

    if (depth % 2 == 0) { // Camera is in air // Normal is ok
        position = hit_point - hit_normal * 0.001f; // small offset to not intersect the last shape
        old_refraction_index = 1;
        new_refraction_index = 1.36f;//shape->getMaterial()->getRefractiveIndex();//TODO change see, if the right normal has been used

    } else {
        
        glm::vec3 hit_normal_inverted = hit_normal;
        /*
        glm::vec3 hit_normal_inverted = hit_normal * -1; // Normal needs to be inverted because the ray enters air again
        //TODO fix binary expression -> const problem!
        */
        
        position = hit_point - hit_normal_inverted * 0.001f; // small offset to not intersect the last shape
        old_refraction_index = 1.36;//shape->getMaterial()->getRefractiveIndex(); // hitting the same shape from the inside//TODO change
        new_refraction_index = 1;

    }

    glm::vec3 refracted_vector = get_refracted(ray.direction, hit_normal, (old_refraction_index / new_refraction_index));


    float roughness = 0.0f;//shape->getMaterial()->getRoughness(); //TODO change
    if (roughness != 0.0) {
        glm::vec3 offset = {0, 0, 0};


        // the first vector doesn't need to be changed, but I did it this way for now

        for (int i = 0; i < 3; ++i) {
            offset[i] =  random_float(roughness);
        }
        offset = glm::normalize(offset) * roughness;
        refracted_vector = refracted_vector + offset;
    }


    Ray refraction_ray = {position, refracted_vector};
    //refractionray.setRefractionindex(shape->getMaterial()->getRefractiveIndex()); //TODO add refraciveindex to ray

    return get_color(refraction_ray, (depth + 1));
}

/**
 * for roughness to get a value between -range and range
 * @param range
 * @return random float value
 */
float Render::random_float(float range) const{
    return ((((float) rand()) / (float) (RAND_MAX / 2.0f)) -1.0f) * range;// range from -range to range
}


/**
 * Function to get a refracted Vector of a normal vector
 * @param vector input
 * @param normal normal of camera
 * @param n n of Material1/ n of Material2
 * @return new refracted vector
 */
glm::vec3 Render::get_refracted(const glm::vec3& vector, glm::vec3 const& normal, float n) const{

    float cos_i = -glm::dot(vector, normal);
    float sin_t2 = (float)(n * n * (1.0 - cos_i * cos_i));
    if (sin_t2 > 1.0) return get_reflected(vector, normal); // TIR
    float cos_t = (float) sqrt(1.0 - sin_t2);

    return vector * n + normal * (n * cos_i - cos_t);
}

/**
 * Function to get a reflected Vector of a normal
 * @param vector input to be reflected
 * @param normal vector to be reflected of
 * @return reflected vector (not normalized)
 */
glm::vec3 Render::get_reflected(const glm::vec3& vector, glm::vec3 const& normal) const{

    float nv = fmax(glm::dot(normal,-vector), 0);//angle
    glm::vec3 normal_non_const = normal;
       /*
       normal *= nv * 2;
       //TODO fix binary expression -> const problem!
       */
    return normal_non_const + vector;
}

