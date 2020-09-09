#include "render.hpp"

/**
 * A method to get the pure color of a point, if an Object has been hit.
 * @param ray
 * @param depth depth of the ray
 */
glm::vec3 Render::get_color (Ray const& ray, int depth) const {
    
    // Get Intersected Object here
    
    glm::vec3 hit_point;
    glm::vec3 hit_normal;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;
    
    composite_->get_intersected_shape (ray, shape, hit_point, hit_normal, dist);
    
    
    // shape has not been hit
    if (dist == INFINITY) {
        return skybox_.get_color (ray.direction);
    }
    
    // get attributes from material
    float glossy = shape->get_material()->glossy;
    float opacity = 1-shape->get_material()->opacity;
    float roughness = shape->get_material()->roughness;

    // The color gets calculated according to the lecture
    glm::vec3 color_final = get_brightness_color (ray, hit_point, hit_normal, depth, shape);
    
    
    // Now we can calculate the reflective and refractive color of the material
    glm::vec3 reflection_color;
    glm::vec3 refraction_color;
    
    // calculates a maximum of n reflections because depth < n
    if (depth < 10) {
        
        // Color calculations reflection
        if (glossy > 0) {
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // roughness = 0 -> number of rays = 1, else number of rays = 1 if depth = 0 else number of rays = 2
            int reflection_samples = (roughness == 0.0f ? 1 : 1 * (depth == 0) + 1);
            // (depth == 0) if true, its a 1, 1*1 + 1 = 2, only if the depth is 0 otherwise its 1
            // n + 1 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only
            for (int i = 0; i < reflection_samples; ++i) {
                reflection_color += get_reflected_color (ray, hit_point, hit_normal, depth, shape); // increasing the depth is not really necessary
            }
            reflection_color *= glossy / (float) reflection_samples;
        }
        
        
        // Color calculations refractions
        if (opacity > 0) {
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // roughness = 0 -> number of rays = 1, else number of rays = 1 if depth = 0 else number of rays = 2
            int refraction_samples = (roughness == 0.0f ? 1 : 1 * (depth == 0) + 1);
            // (depth == 0) if true, its a 1, 1*1 + 1 = 2, only if the depth is 0 otherwise its 1
            // n + 1 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only
            
            for (int i = 0; i < refraction_samples; ++i) {
                refraction_color += get_refracted_color(ray, hit_point, hit_normal, depth,
                                                        shape);// increasing the depth is not really necessary
            }
            refraction_color *= opacity / (float) refraction_samples;
        }
    }
    
    
    // Color mixing for different opacity and glossyness values
    color_final *= (1.0f - opacity);
    color_final *= (1.0f - glossy);
    
    color_final += reflection_color + refraction_color;
    
    return color_final / glm::length(glm::vec3{1 - opacity, opacity, glossy});
}

/**
 * Set the composite object for this Renderer
 * All shapes inside the composite object will be drawn.
 * @param composite_1 object with the information to be rendered
 */
void Render::set_composite (std::shared_ptr<Composite> const& composite_1) {
    composite_ = composite_1;
}

/**
 Set the lights for this Renderer
 @param lights_1 lights vector with the information to be rendered
 */
void Render::set_lights (std::vector<std::shared_ptr<Light>> const& lights_1) {
    lights_ = lights_1;
}

/**
 Set the ambient light for this Renderer
 @param ambient
 */
void Render::set_ambient_scene (glm::vec3 const& ambient) {
    ambient_scene_ = ambient;
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
glm::vec3 Render::get_brightness_color (Ray const &ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth,
                                        std::shared_ptr<Shape> const& shape) const {
    
    // ambient light not visible because it doesnt hit anything
    glm::vec3 intensity_color = shape->get_material()->color_ambient * ambient_scene_;
    glm::vec3 specular;
    glm::vec3 diffuse;
    
    float light_distance;
    float angle;
    glm::vec3 hit_to_light;
    
    for (int i = 0; i < lights_.size(); ++i) {
        
        hit_to_light = (lights_[i]->position - hit_point);
        
        light_distance = glm::length (hit_to_light);
        hit_to_light = glm::normalize (hit_to_light);
        
        angle = glm::dot(hit_normal, hit_to_light);
        
        // normal facing in direction of light
        if (0 < angle && depth < 20) {
            
            // Specular:
            glm::vec3 mirror_to_light = glm::normalize(get_reflected_vec3(-hit_to_light, hit_normal));
            float stength_specular = glm::dot(-ray.direction, mirror_to_light);
            stength_specular = (float) std::pow(stength_specular, shape->get_material()->reflective_exponent);
            
            specular = shape->get_material()->color_specular * stength_specular;
            
            
            // Diffuse:
            diffuse = shape->get_material()->color_diffuse * angle + specular;
            diffuse *= lights_[i]->brightness;
            
            
            // New Shadow calculations
            // No. of samples = important if range != 0
            
            glm::vec3 offset = glm::vec3{0.0f, 0.0f, 0.0f};
            // new position to shoot a ray from to not hit the last shape hit instantly
            glm::vec3 newpos = hit_point + hit_normal * 0.0001f;
            
            // for soft shadows
            if (lights_[i]->hardness < 1.0f) {
                offset = glm::normalize (glm::vec3{random_float(), random_float(), random_float()}) * (1.0f - lights_[i]->hardness);
            }
            
            // offset is now a vec3 with a random direction and the length of roughness
            offset += glm::normalize(hit_to_light) * 0.001f;
            
            glm::vec3 new_hit_point;
            glm::vec3 new_hit_normal;
            float new_dist = INFINITY;
            std::shared_ptr<Shape> new_shape;
            
            composite_->get_intersected_shape ({newpos, offset}, new_shape, new_hit_point,new_hit_normal, new_dist);
            
            // light is closer than anything hit
            if (INFINITY == new_dist || light_distance < new_dist) {
                
                // The original hit_point is in the light
                intensity_color += diffuse/(light_distance * light_distance);
                
            }
        }
    }
    
    return intensity_color;
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
glm::vec3 Render::get_refracted_color (Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth,
                            std::shared_ptr<Shape> const& shape) const {
    
    glm::vec3 total_color;
    
    int max = 3; // for red, green and blue (for chromatic aberration)
    int min = 0; // start
    
    if (depth > 1 || shape->get_material()->aberration_strength <= 0) {
        min = 1; // only one color value will be calculated
        max = 2; // only one color value will be calculated
    }
    
    for (int i = min; i < max; ++i) {
        
        float aberration = (float) i - 1;
        // scale of the aberration
        aberration *= -shape->get_material()->aberration_strength;
    
        glm::vec3 position;
        glm::vec3 refracted_vector;
        
        // Camera is in air, Normal is ok
        if (depth % 2 == 0) {
            // small offset to not intersect the last shape
            position = hit_point - hit_normal * 0.0001f;
            // the 1 is the refractive index of air (could be changed to any medium if we wanted)
            refracted_vector = get_refracted_vec3 (ray.direction, hit_normal, (1 / (shape->get_material()->refractive_index + aberration)));
        } else {
            // small offset to not intersect the last shape
            position = hit_point + hit_normal * 0.0001f;
            // the 1 is the refractive index of air (could be changed to any medium if we wanted)
            refracted_vector = get_refracted_vec3(ray.direction, -hit_normal, ((shape->get_material()->refractive_index + aberration) / 1));
        }
        
        if (shape->get_material()->roughness != 0.0f) {
            // Adds a vector in a random direction with the length of the roughness
            refracted_vector += glm::normalize (glm::vec3{random_float(), random_float(), random_float()}) * shape->get_material()->roughness;
        }
        
        glm::vec3 color = get_color({position, refracted_vector}, depth + 1);
        total_color[i] = color[i];
        
        if (depth > 1 || shape->get_material()->aberration_strength == 0.0f) {
            // no chromatic aberation at all
            total_color = color;
        }
        
    }
    return total_color;
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
Render::get_reflected_color (Ray const& ray, glm::vec3 const& hit_point, glm::vec3 const& hit_normal, int depth,
                            std::shared_ptr<Shape> const& shape) const {
    
    glm::vec3 reflected_vector = get_reflected_vec3(ray.direction, hit_normal);
    
    // no roughness = no multiple rays to get a mat material effect
    if (shape->get_material()->roughness != 0.0f) {
        glm::vec3 offset = {0.0f, 0.0f, 0.0f};
        
        // the first vector does not need to be changed, but I did it this way for now
        for (int i = 0; i < 3; ++i) {
            offset[i] = random_float();
        }
        
        offset = glm::normalize(offset) * shape->get_material()->roughness;
        reflected_vector = reflected_vector + offset;
    }
    
    Ray reflection_ray {hit_point + hit_normal * 0.0001f, reflected_vector};
    
    return get_color(reflection_ray, depth);
}

/**
 * for roughness to get a value between -1 and 1
 * @return random float value
 */
float Render::random_float() const {
    // range from -range to range
    return ((((float) rand()) / (float) (RAND_MAX / 2.0f)) - 1.0f);
}


/**
 * Function to get a refracted Vector of a normal vector
 * @param vector input
 * @param normal normal of camera
 * @param n n of Material1/ n of Material2
 * @return new refracted vector
 */
glm::vec3 Render::get_refracted_vec3 (glm::vec3 const& vector, glm::vec3 const& normal, float n) const {
    
    float cos_i = -glm::dot(normal, vector);
    float sin_t2 = (n * n * (1.0f - (float) (cos_i * cos_i)));
    if (sin_t2 > 1.0f) return get_reflected_vec3(vector, normal);
    return vector * n + (normal * (float) (n * cos_i - sqrt(1.0f - sin_t2)));
}

/**
 * Function to get a reflected Vector of a normal
 * @param vector input to be reflected
 * @param normal vector to be reflected of
 * @return reflected vector (not normalized)
 */
glm::vec3 Render::get_reflected_vec3 (glm::vec3 const& vector, glm::vec3 const& normal) const {
    return normal * (float) (2.0f * fmax(glm::dot(normal, -vector), 0.0f)) + vector;
}


