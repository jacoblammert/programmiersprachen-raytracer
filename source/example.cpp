#include "fensterchen.hpp"

int main(int argc, char* argv[])
{

  Window win{glm::ivec2{800,800}};

  while (!win.should_close()) {
    if (win.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      win.close();
    }

    auto t = win.get_time();
    auto m = win.mouse_position();


    win.update();
  }

  return 0;
}


/*
 
 
 #include <iostream> // debug prints
 #include <fstream>  // open / read / write /close files
 #include <sstream>  //stringstream -> easy parsing mechanics
 #include <string>   // buffer lines read from file
 
 
int main(int argc, char** argv) {

  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " <PATH_TO_SDF_FILE:string>" << std::endl;
    
    return -1;
  }

  std::string const in_file_path = argv[1];

  // open file in read-only && ASCII mode
  std::ifstream in_file(in_file_path, std::ios::in);
  
  std::string line_buffer;

  int32_t line_count = 0;


  std::string identifier;
  std::string class_name;

  while( std::getline(in_file, line_buffer)  ) {
    //std::cout << ++line_count << ": " << line_buffer << std::endl;
    
    //construct stringstream using line_buffer string
    std::istringstream in_sstream(line_buffer);
    
    in_sstream >> identifier;

    std::cout << "Identifier content: " << identifier << std::endl;
    
    // check for shapes / materials / lights
    if("define" == identifier) {
      in_sstream >> class_name;
  
      // check for specific shape
      if("shape" == class_name) {
         //check for shape type, then: parse attributes (including material lookup)
      } else if ("material" == class_name) {
        //parse material attributes
        std::string material_name;
        float ka_red, ka_green, ka_blue;
        float kd_red, kd_green, kd_blue;
        float ks_red, ks_green, ks_blue;
        float m;
 
        in_sstream >> material_name;
        in_sstream >> ka_red >> ka_green >> ka_blue; //
 
        std::cout << ka_red  << " " << ka_green << " " << ka_blue << std::endl;

      } else if ("light" == class_name) {

      } else {
        std::cout << "Line was not valid!" << std::endl;
      }
    }

  }
  
  // close file
  in_file.close();
    
    Window win{glm::ivec2{800,800}};

    while (!win.should_close()) {
      if (win.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        win.close();
      }

      auto t = win.get_time();
      auto m = win.mouse_position();

      win.update();
    }
    
  return 0;
}



*/




