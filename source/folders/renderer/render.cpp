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
glm::vec3 Render::getColor(Ray ray, int depth) const{



    //TODO split functions into different subfunctions to make visibility better




    /**Get Intersected Object here**/

    glm::vec3 HitPoint;
    glm::vec3 HitNormal;
    bool hitbool = false;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;

    composite->get_intersected_shape(ray,shape,HitPoint,HitNormal,dist,hitbool); // TODO pass pointer instead of all the parameters



    if (!hitbool) { // shape gets hit
        //TODO implement skybox or background
        return {}; // returns black, if no shape has been hit
    }


    float glossy = 0.0f;//shape->getMaterial()->getGlossy();
    float transparency = 0.0f;//shape->getMaterial()->getTransparency();
    float roughness = 0;//shape->getMaterial()->getRoughness();


    glm::vec3 colorfinal{1,1,1};//shape->getMaterial()->getColor()};

    colorfinal = colorfinal  * (0.2f * (1 - transparency));


    if (!(transparency == 1.0f || glossy == 1.0f)) {///*/true/*/(shapes[hit]->getMaterial()->getTransparency() != 1.0f /*/||/*/&&/**/ /*/shapes[hit]->getMaterial()->getGlossy() != 1.0f/*/ true/**/)/**/) { // ||?
        float lightStrength = 0;


        glm::vec3 HitToLight = {};
        for (int i = 0; i < lights.size(); ++i) {

            if (lights[i]->brightness > 0) {

                glm::vec3 Normal = HitNormal;
                //Normal = glm::normalize(Normal);


                HitToLight = (lights[i]->position - HitPoint);
                lightStrength = glm::length(HitToLight);
                HitToLight = glm::normalize(HitToLight);

                float angle = glm::dot(Normal,HitToLight);

                if (0 < angle) { // normal facing in direction of light
                    Normal *= 0.1f;
                    if (!castShadowRay({HitPoint + Normal, HitToLight}, lightStrength)) {

                        glm::vec3 Lightcolor = (lights[i]->color * (angle * lights[i]->brightness)) / (lightStrength * lightStrength); // brightness

                        colorfinal += Lightcolor;//(shape->getMaterial()->getColor() * Lightcolor); //TODO change in one line or with material
                    }
                }
            }
        }
    }


    glm::vec3 reflectionColor;
    glm::vec3 refractionColor;

    if (depth < 20) { // calculates as n reflections because depth < n

        //glm::vec3 Normal{HitNormal};//HitNormal.scale(0.001);


        if (glossy > 0) {
            int reflectionsamples = roughness == 0.0f ? 1 : 3 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < reflectionsamples; ++i) {
                reflectionColor += getReflectedColor(ray, HitPoint, HitNormal, depth, shape);// increasing the depth is not really necessary
            }
            reflectionColor *= glossy / (float) reflectionsamples;
        }

        if (transparency > 0) {
            int refractionsamples = roughness == 0.0f ? 1 : 3 * (depth == 0) + 1;
            // max number of rays is 4, if the depth (iteration) is greater than 1, we do only have one new ray
            // 4 rays in the first iteration, only 1 in each following iteration. If the roughness is 0.0f, we do always have one new ray only

            for (int i = 0; i < refractionsamples; ++i) {
                refractionColor += getRefractedColor(ray, HitPoint, HitNormal, depth,
                                                     shape);// increasing the depth is not really necessary
            }
            refractionColor *= transparency / (float) refractionsamples;
        }
    }

    colorfinal *= (1.0f - transparency);

    colorfinal += (reflectionColor + refractionColor);

    float value = 1 - transparency;

    glm::vec3 Brightness{value, transparency, glossy};

    colorfinal /= glm::length(Brightness);

    return colorfinal;
}

/**
 * returns false, if a ray sent in the light direction has intersected with a shape
 * @param ray to fly to the lightsource
 * @param distance to the light, if it hits anything closer, we have shadow
 * @return intersection further than distance
 */
bool Render::castShadowRay(const Ray& ray, float distance) const{


    glm::vec3 Hitpoint;
    glm::vec3 HitNormal;
    bool hitbool = false;
    float dist = INFINITY;
    std::shared_ptr<Shape> shape;


    composite->get_intersected_shape(ray, shape, Hitpoint,HitNormal,dist,hitbool);
    //TODO clean up
    return hitbool && dist < distance && dist >= 0;
}


/**
 * for roughness to get a value between -range and range
 * @param range
 * @return random float value
 */
float Render::randomFloat(float range) const{
    float r = ((float) rand()) / (float) (RAND_MAX / 2.0f);
    r -= 1.0f; // range from -1 to 1
    r *= range;// range from -range to range
    return r;
}


/**
 * Function to calculate the color of the reflected ray
 * @param ray will be reflected and sent away
 * @param HitPoint Point where the old ray has intersected the shape
 * @param HitNormal normal at the intersected position
 * @param depth of the ray to set a recursion depth
 * @param shape pointer of the last intersected shape (for material/ refractive index information later on)
 * @return the calculated color after all recursions came to an end
 */
glm::vec3 Render::getReflectedColor(Ray ray, glm::vec3 HitPoint, glm::vec3 HitNormal, int depth, std::shared_ptr<Shape> shape/*** dont remove, we need the roughness*/) const{

    glm::vec3 reflectedVector = getReflected(ray.direction,HitNormal);


    float roughness = 0;//shape->getMaterial()->getRoughness();//TODO change
    if (roughness != 0.0) { // no roughness = no multiple rays to get a mat material effect
        glm::vec3 offset = {0, 0, 0};

        // the first vector doesent need to be changed, but I did it this way for now

        for (int i = 0; i < 3; ++i) {
            offset[i] = randomFloat(roughness);
        }
        offset = glm::normalize(offset) * roughness;
        reflectedVector = reflectedVector + offset;
    }


    Ray reflectionray = {HitPoint + HitNormal * 0.001f, reflectedVector};

    return getColor(reflectionray, depth);
}

/**
 * Function to calculate the color of the refracted ray
 * @param ray stores information about the refractive index of the new/ old object to make the correct calculations
 * the ray does have a refractive index of 1.0 in the beginning regardless of its actual position -> wrong calculations when
 * the camera starts inside an object/ two shapes intersect
 * @param HitPoint Point where the old ray has intersected the shape
 * @param HitNormal normal at the intersected position
 * @param depth of the ray to set a recursion depth
 * @param shape pointer of the last intersected shape (for material/ refractive index information)
 * @return the calculated color after all recursions came to an end
 */
glm::vec3 Render::getRefractedColor(Ray ray, glm::vec3 HitPoint, glm::vec3 HitNormal, int depth, std::shared_ptr<Shape> shape/*** dont remove, we need the reflective index*/) const{

    glm::vec3 position = {};

    float newRefractionindex;
    float oldrefractionIndex;

    if (depth % 2 == 0) { // Camera is in air // Normal is ok
        position = HitPoint - HitNormal * 0.001f; // small offset to not intersect the last shape
        oldrefractionIndex = 1;
        newRefractionindex = 1.36f;//shape->getMaterial()->getRefractiveIndex();//TODO change see, if the right normal has been used

    } else {
        HitNormal *= -1;// Normal needs to be inverted because the ray enters air again
        position = HitPoint - HitNormal * 0.001f; // small offset to not intersect the last shape
        oldrefractionIndex = 1.36;//shape->getMaterial()->getRefractiveIndex(); // hitting the same shape from the inside//TODO change
        newRefractionindex = 1;

    }

    glm::vec3 refractedVector = getRefracted(ray.direction,HitNormal, oldrefractionIndex / newRefractionindex);


    float roughness = 0.0f;//shape->getMaterial()->getRoughness(); //TODO change
    if (roughness != 0.0) {
        glm::vec3 offset = {0, 0, 0};


        // the first vector doesn't need to be changed, but I did it this way for now

        for (int i = 0; i < 3; ++i) {
            offset[i] =  randomFloat(roughness);
        }
        offset = glm::normalize(offset) * roughness;
        refractedVector = refractedVector + offset;
    }


    Ray refractionray = {position, refractedVector};
    //refractionray.setRefractionindex(shape->getMaterial()->getRefractiveIndex()); //TODO add refraciveindex to ray

    return getColor(refractionray, depth + 1);
}

/**
 * Set the composite object for this Renderer
 * All shapes inside the composite object will be drawn.
 * @param composite object with the information to be rendered
 */
void Render::setComposite(std::shared_ptr<Composite> composite1) {
    this->composite = std::move(composite1);
}

void Render::setLights(std::vector<std::shared_ptr<Light>> lights1) {
    this->lights = std::move(lights1);
}

/**
 * Function to get a reflected Vector of a normal
 * @param vector input to be reflected
 * @param normal vector to be reflected of
 * @return reflected vector (not normalized)
 */
glm::vec3 Render::getReflected(const glm::vec3& vector, glm::vec3 normal) const{

    float NV = fmax(glm::dot(normal,-vector), 0);//angle
    normal *= NV * 2;
    return normal + vector;
}

/**
 * Function to get a refracted Vector of a normal vector
 * @param vector input
 * @param normal normal of camera
 * @param n n of Material1/ n of Material2
 * @return new refracted vector
 */
glm::vec3 Render::getRefracted(const glm::vec3& vector, const glm::vec3& normal, float n) const{

    float cosI = -glm::dot(vector,normal);
    float sinT2 = (float)(n * n * (1.0 - cosI * cosI));
    if (sinT2 > 1.0) return getReflected(vector,normal); // TIR
    float cosT = (float) sqrt(1.0 - sinT2);

    return vector * n + normal * (n * cosI - cosT);
}


