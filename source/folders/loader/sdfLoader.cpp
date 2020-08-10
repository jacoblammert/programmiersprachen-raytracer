//
// Created by Jacob on 10.08.2020.
//

#include "sdfLoader.h"
#include <iostream>
#include <fstream>
#include <sstream> // string stream -> easy parsing mechanics
#include <string>

sdfLoader::sdfLoader(std::string filepath) {

    if (filepath.length() > 4){
        loadFile(filepath);
    }
}

void sdfLoader::loadFile(std::string filepath) {

    //open file in read-only && ASCII mode
    std::ifstream in_file(filepath,std::ios::in);
    std::string line_buffer;
    int32_t line_count = 0;

    std::string identifier;
    std::string class_name;

    while (std::getline(in_file,line_buffer)){
        std::cout<<++line_count << line_buffer<< std::endl;




        std::istringstream  in_sstream(line_buffer);

        in_sstream >> identifier;

        //std::cout<<"Identifier content: "<< identifier << std::endl;

        if("define" == identifier){
            in_sstream >> class_name;
            if ("shape" == class_name){
                std::string material_name;
                float ka_red, ka_green, ka_blue;
                float kd_red, kd_green, kd_blue;
                float ks_red, ks_green, ks_blue;
                float m;

                in_sstream >> material_name;
                in_sstream >> ka_red, ka_green, ka_blue;// kd_red,kd_green,kd_blue

            }
        }





    }


    //close file
    in_file.close();


}
