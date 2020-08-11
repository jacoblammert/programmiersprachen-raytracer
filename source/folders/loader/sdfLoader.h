//
// Created by Jacob on 10.08.2020.
//

#ifndef RAYTRACER_SDFLOADER_H
#define RAYTRACER_SDFLOADER_H


#include <string>

class sdfLoader {
public:
    sdfLoader(std::string filepath);

private:
    void loadFile()const;
    std::string filepath;
};


#endif //RAYTRACER_SDFLOADER_H
