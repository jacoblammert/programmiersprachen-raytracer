//
// Created by Jacob Lammert on 17.08.2020.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "shape.h"

class triangle : public shape {
public:
    triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);

    //Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, Color color);


    bool getIntersectVec(ray ray, glm::vec3 &HitPoint, glm::vec3 &HitNormal, float &distance) const override;

    glm::vec3 getNormal(glm::vec3 pos) const override;

    glm::vec3 getMin() const override;

    glm::vec3 getMax() const override;

    glm::vec3 getMedian() const override;

/*/
    Material* getMaterial() override;

    void setMaterial(Material* material) override;
/**/

    void translate(glm::vec3 position) override;

    void print() const override;

private:
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 normal;


};


#endif //RAYTRACER_TRIANGLE_H
