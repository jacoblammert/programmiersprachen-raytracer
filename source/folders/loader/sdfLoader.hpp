//
// Created by Jacob on 10.08.2020.
//

#ifndef RAYTRACER_SDFLOADER_H
#define RAYTRACER_SDFLOADER_H


#include <string>
#include <glm-0.9.5.3/glm/vec3.hpp>

class SdfLoader {
public:
    SdfLoader(std::string filepath);

    void loadFile() const;

private:

    std::string filepath;
};


#endif //RAYTRACER_SDFLOADER_H
