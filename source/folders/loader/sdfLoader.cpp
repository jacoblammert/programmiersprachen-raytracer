//
// Created by Jacob on 10.08.2020.
//

#include "sdfLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream> // string stream -> easy parsing mechanics
#include <string>

SdfLoader::SdfLoader(std::string filepath) :
    filepath {filepath}
{}

void SdfLoader::loadFile() const { //const correctness valid?

    if (filepath.empty()) {
        std::cout << "Please set a valid filepath" << std::endl;
        return;
    }

    //open file in read-only && ASCII mode
    std::ifstream in_file(filepath, std::ios::in);
    std::string line_buffer;
    int32_t line_count = 0;

    std::string identifier;
    std::string class_name;

    while (std::getline(in_file, line_buffer)) {
        std::cout << ++line_count << line_buffer << std::endl;

        //construct stringstream using line_buffer string
        std::istringstream in_sstream(line_buffer);

        in_sstream >> identifier;

        std::cout << "Identifier content: " << identifier << std::endl;

        //check for shapes/ materials/ lights
        if ("define" == identifier) {
            in_sstream >> class_name;
            //check for specific shape
            if ("shape" == class_name) {
                //check for shape type, then parse attributes (including material lookup)
            } else if ("material" == class_name) {
                //parse material attributes
                std::string material_name;
                float ka_red, ka_green, ka_blue;
                float kd_red, kd_green, kd_blue;
                float ks_red, ks_green, ks_blue;
                float m;

                in_sstream >> material_name;
                in_sstream >> ka_red >> ka_green >> ka_blue;// kd_red,kd_green,kd_blue
                
                std::cout << ka_red << ", " << ka_green << ", " << ka_blue << std::endl;
                
            } else if ("light" == class_name) {

            } else if ("camera" == class_name) {

            } else {
                std::cout << "Line was not valid!" << std::endl;
            }
        }
    }


    //close file
    in_file.close();
}
