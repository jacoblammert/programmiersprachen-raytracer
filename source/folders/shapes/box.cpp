//
// Created by Jacob Lammert on 17.08.2020.
//

#include "box.hpp"


/**
 * box constructor for axis alligned box with a min and max vector for example: {-1,-1,-1} {1,1,1}
 * @param minXminYminZ minimum of the box (individual values must be smaller than their counterpart in max vector)
 * @param maxXmaxYmaxZ maximum of the box (individual values must be greater than their counterpart in min vector)
 */
Box::Box(const glm::vec3 &minXminYminZ, const glm::vec3 &maxXmaxYmaxZ) {

    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);

    position = minXminYminZ + maxXmaxYmaxZ;
    position *= 0.5;
}

/**
 * box constructor for axis alligned box with a pos vector (middle of the box) and sizes for x, y and z as floats
 * @param Pos position (middle) of the box
 * @param xScale float for size in x direction
 * @param yScale float for size in y direction
 * @param zScale float for size in z direction
 */
Box::Box(glm::vec3 const& Pos, float xScale, float yScale, float zScale) { //Warum Pos statt pos?
    glm::vec3 minXminYminZ = {Pos[0] - xScale / 2, Pos[1] - yScale / 2, Pos[2] - zScale / 2};
    glm::vec3 maxXmaxYmaxZ = {Pos[0] + xScale / 2, Pos[1] + yScale / 2, Pos[2] + zScale / 2};
    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);
    position = Pos;
}
/*///TODO additional constructors with materials
Box::Box(Vector const& minXminYminZ, Vector const& maxXmaxYmaxZ, Color const& color) {
    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);

    position = minXminYminZ + maxXmaxYmaxZ;
    position.scale(0.5);
}

Box::Box(Vector const& Pos, float xScale, float yScale, float zScale, Color const& color) {
    Vector minXminYminZ = Vector(Pos.getX() - xScale / 2, Pos.getY() - yScale / 2, Pos.getZ() - zScale / 2);
    Vector maxXmaxYmaxZ = Vector(Pos.getX() + xScale / 2, Pos.getY() + yScale / 2, Pos.getZ() + zScale / 2);
    bounds.push_back(minXminYminZ);
    bounds.push_back(maxXmaxYmaxZ);
    position = Pos; //vorherigen Konstruktor nutzen?
}/**/


/**
 * returns true, if the box was in front of the ray and the ray intersected the box
 * @param ray to be tested
 * @param HitPoint position where the ray intersects the box first (for two intersections the closest point will be returned) as reference
 * @param HitNormal The normal of the box at the intersected position as reference
 * @param distance from the normalized ray to the intersection point as reference
 * @return true, of the box is in front of the ray and has been intersected
 */
bool Box::getIntersectVec(Ray const& ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const { //sinnvoll HitBox returnen?

    glm::vec3 rayposition = ray.position;
    glm::vec3 raydirection = ray.direction;
    raydirection = {1 / raydirection[0], 1 / raydirection[1], 1 / raydirection[2]};

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[sign(raydirection, 0)][0] - rayposition[0]) * raydirection[0]; //txmin?
    tmax = (bounds[1 - sign(raydirection, 0)][0] - rayposition[0]) * raydirection[0]; //tymin?

    tymin = (bounds[sign(raydirection, 1)][1] - rayposition[1]) * raydirection[1];
    tymax = (bounds[1 - sign(raydirection, 1)][1] - rayposition[1]) * raydirection[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign(raydirection, 2)][2] - rayposition[2]) * raydirection[2];
    tzmax = (bounds[1 - sign(raydirection, 2)][2] - rayposition[2]) * raydirection[2];

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;


    if (0 < tmin && tmin < distance) {
        raydirection = ray.direction;
        raydirection *= tmin;

        distance = tmin;
        HitPoint = rayposition + raydirection;
        HitNormal = getNormal(HitPoint);
        return true;
    }
    if (tmin < 0 && 0 < tmax && tmax < distance) {
        raydirection = ray.direction;
        raydirection *= tmax;

        distance = tmax;
        HitPoint = rayposition + raydirection;
        HitNormal = getNormal(HitPoint);
        return true;
    }


}


/**
 * Important for BoundingBox intersection testing. Similar to the previous method, but without all the useless stuff (could be faster)
 * @param ray that gets tested against this box
 * @return true if the ray hits
 */
bool Box::getIntersect(const Ray &ray) const {

    glm::vec3 rayposition = ray.position;
    glm::vec3 raydirection = ray.direction;
    raydirection = {1 / raydirection[0], 1 / raydirection[1], 1 / raydirection[2]};


    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[sign(raydirection, 0)][0] - rayposition[0]) * raydirection[0];
    tmax = (bounds[1 - sign(raydirection, 0)][0] - rayposition[0]) * raydirection[0];

    tymin = (bounds[sign(raydirection, 1)][1] - rayposition[1]) * raydirection[1];
    tymax = (bounds[1 - sign(raydirection, 1)][1] - rayposition[1]) * raydirection[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;


    if (tymin > tmin)
        tmin = tymin;


    if (tymax < tmax)
        tmax = tymax;


    tzmin = (bounds[sign(raydirection, 2)][2] - rayposition[2]) * raydirection[2];
    tzmax = (bounds[1 - sign(raydirection, 2)][2] - rayposition[2]) * raydirection[2];

    return !((tmin > tzmax) || (tzmin > tmax));
}

/**
 * returns the normal for a given position
 * @param position position (Hitposition)
 * @return normal at the given position. Either {1,0,0},{0,1,0},{0,0,1} or {-1,0,0},{0,-1,0},{0,0,-1}
 */
glm::vec3 Box::getNormal(glm::vec3 const& pos) const {

    float epsilon = 0.000001f; // for edges/ corners -> uncertainty

    if (pos[0] <= bounds[0][0] + epsilon) {
        return {-1, 0, 0}; // back
    } else if (pos[0] >= bounds[1][0] - epsilon) {
        return {1, 0, 0}; // front
    }
    
    if (pos[1] <= bounds[0][1] + 0.001) { //warum nicht auch + epsilon?
        return {0, -1, 0}; // left
    } else if (pos[1] >= bounds[1][1] - epsilon) {
        return {0, 1, 0}; // right
    }
    
    if (pos[2] <= bounds[0][2] + epsilon) {
        return {0, 0, -1}; // bottom
    } else if (pos[2] >= bounds[1][2] - epsilon) {
        return {0, 0, 1}; // top
    }
    
    return {0, 1, 0}; // if in corner, or something is wrong
}


/**
 * @return a vector with the minimal values of x, y and z of the box
 */
glm::vec3 Box::getMin() const {
    return bounds[0];
}


/**
 * @return a vector with the maximal values of x, y and z of the box
 */
glm::vec3 Box::getMax() const {
    return bounds[1];
}

/**
 * @return the position of the box
 */
glm::vec3 Box::getMedian() const { //besser getPosition?
    return position;
}

/**
 * outputs important information in the console
 */
void Box::print() const {
    std::cout << "Box: " << std::endl;
    std::cout << "Min: ";
    //bounds[0].print();
    std::cout << "Max: ";
    //bounds[1].print();

    //TODO add print function
}

/*/
Material *Box::getMaterial() {
    return material;
}

void Box::setMaterial(Material *material) {
    this->material = material;
}/**/

/**
 * Changes the position of the Sphere with a given glm::vec3
 * @param position
 */
void Box::translate(glm::vec3 const& position) {
    for (int i = 0; i < bounds.size(); ++i) {
        bounds[i] += position;
    }
    this->position += position;
}

/**
 * function to return 1 or 0 for the given comparison
 * for more information visit scratchapixel + box intersection method
 * @param vec3 position
 * @param pos in the vector
 * @return 0 or 1
 */
int Box::sign(glm::vec3 const& vec3, int pos) const {
    return ((pos == 0 && vec3[0] < 0) || (pos == 1 && vec3[1] < 0) || (pos == 2 && vec3[2] < 0));
}
