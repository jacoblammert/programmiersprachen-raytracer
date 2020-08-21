//
// Created by Jacob Lammert on 20.08.2020.
//

#ifndef RAYTRACER_COMPOSITE_HPP
#define RAYTRACER_COMPOSITE_HPP


#include <vector>
#include <memory>
#include "shape.hpp"
#include "box.hpp"

/**
 * This object is like a Kd-Tree and saves all the Shape pointers in a Vector.
 * The intersectionfunctions are different because the box must be intersected before any of the included Shapes are tested
 *
 *
 */
class Composite : public Shape {
public:

    Composite();

    Composite(int depth);

    Composite(std::vector<std::shared_ptr<Shape>> shapes);

    bool getIntersectVec(Ray const &ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    void getIntersectedShape(const Ray &ray, std::shared_ptr<Shape> shape, glm::vec3 &Hitpoint, glm::vec3 &Hitnormal, float &distance,
                             bool &hit);

    glm::vec3 getNormal(glm::vec3 const &pos) const override;

    glm::vec3 getMin() const override;

    glm::vec3 getMax() const override;

    glm::vec3 getMedian() const override;


    void translate(glm::vec3 const &position) override;

/*/ /// The compositeobject does not require a material at all
    Material* getMaterial() override;
    void setMaterial(Material* material) override;
/**/
    void print() const override;

    void build();

    void addShape(std::shared_ptr<Shape> shape);

    void addShapes(std::vector<std::shared_ptr<Shape>> shapes);

private:

    void split();

    void setMinMaxMid();

    void getMin(glm::vec3 shapemin);

    void getMax(glm::vec3 shapemax);

    glm::vec3 minXminYminZ;
    glm::vec3 maxXmaxYmaxZ;
    glm::vec3 median;

    int depth;

    Box box;


    std::vector<Composite> boxes; // TODO change from vector to two seperate objects

    std::vector<std::shared_ptr<Shape>> shapes;
};


#endif //RAYTRACER_COMPOSITE_HPP
