//
// Created by Jacob Lammert on 01.09.2020.
//

#include <glm-0.9.5.3/glm/glm.hpp>
#include <fstream>
#include "ppm.hpp"

Ppm::Ppm() {

}

void Ppm::load(const std::string& filename) {

    /// Reads all the lines in one vector ("splits" by \n)
    std::ifstream inputFile(filename);        // Input file stream object
    std::vector<std::string> filestring = std::vector<std::string>();

    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // using printf() in all tests for consistency
            //printf("%s", line.c_str());
            filestring.push_back(line);
        }
        file.close();
    }

    /// splits the vector at " " spaces
    std::string token = " ";
    std::vector<std::string> result;
    for (int i = 0; i < filestring.size(); ++i) {

        while (!filestring[i].empty()) {

            int index = filestring[i].find(token);

            if (index != std::string::npos) {
                result.push_back(filestring[i].substr(0, index));
                filestring[i] = filestring[i].substr(index + token.size());
                if (filestring[i].empty())
                    result.push_back(filestring[i]);
            } else {
                result.push_back(filestring[i]);
                filestring[i] = "";
            }
        }
    }
    filestring = result;

    /// Construction of the loaded image as glm::vec3 in a two dimensional vector


    int x = 0;
    int y = 0;

    for (int i = 0; i < filestring.size(); ++i) {
        if (filestring[0] == "P3") {

            if (i < 4) {
                if (i == 1) {
                    width = std::stoi(filestring[i]);
                } else if (i == 2) {
                    height = std::stoi(filestring[i]);
                    std::vector<glm::vec3> vec;
                    vec.assign(width, {});
                    image.assign(height, vec);
                }
            } else {
                if (i % 3 == 1) {

                    x = ((i - 2) / 3) % width;

                    image[y][x][0] = std::stoi(filestring[i]) / 255.0f;
                } else if (i % 3 == 2) {
                    image[y][x][1] = std::stoi(filestring[i]) / 255.0f;
                } else {
                    image[y][x][2] = std::stoi(filestring[i]) / 255.0f;

                    if (((i - 1) / 3) % width == 0) {
                        y++;
                    }
                    y = y % height;
                }
            }
        }
    }
}

glm::vec3 Ppm::get_pixel(glm::vec2 position) const {
    if (width < position[0]){
        position[0] = width;
    }
    if (height < position[1]){
        position[1] = height;
    }

    if (0 <= position[0] && 0 <= position[1]) {
        return image[position[1]][position[0]];
    } else{
        return {};
    }
}
