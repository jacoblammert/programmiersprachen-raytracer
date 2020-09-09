
#include <fstream>
#include "objLoader.hpp"

objLoader::objLoader() {

}

std::vector<std::shared_ptr<Shape>> objLoader::load_obj(std::string filename) {


    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<glm::vec3> vectors;
    std::vector<std::string>filestring;


    std::vector<int> numbers;
    std::ifstream inputFile(filename);        // Input file stream object

    // Read all the lines from the file

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

    // Split the strings in the vector by spaces

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



    // We can now import the the triangles with the specified values the way the .obj file works
    // o
    // objectname
    // v
    // all the vertices below (x, y, z) (Triangulated)
    // f
    // all the faces (position of vertices in the given order) e.g 2 1 7  -> a triangle with the points at Position 2, 1 and 7 in the vertex list
    int A, B, C;

    std::string objectname = "empty";

    for (int i = 0; i < filestring.size(); ++i) {
        if (filestring[i] == "o") {
            objectname = filestring[i + 1]; // We can only get one object name this way
        } else if (filestring[i] == "v") {
            vectors.emplace_back(std::stof(filestring[i + 1]),
                                         std::stof(filestring[i + 2]),
                                         std::stof(filestring[i + 3]));
        } else if (filestring[i] == "f") {

            A = (int) std::stof(filestring[i + 1]);
            B = (int) std::stof(filestring[i + 2]);
            C = (int) std::stof(filestring[i + 3]);

            shapes.push_back(std::make_shared<Triangle>(Triangle{objectname + std::to_string(i),vectors[A-1], vectors[B-1], vectors[C-1]}));
        }
    }

    return shapes;
}
