#include "sdfLoader.hpp"
#include "../shapes/triangle.hpp"

SdfLoader::SdfLoader(std::string filepath) :
        filepath_ {std::move(filepath)}
{}

void SdfLoader::load_file() const { //const correctness valid?

    if (filepath_.empty()) {
        std::cout << "Please set a valid filepath" << std::endl;
        return;
    }

    //open file in read-only && ASCII mode
    std::ifstream in_file(filepath_, std::ios::in);
    std::string line_buffer;
    int32_t line_count = 0;

    std::string identifier;


    std::shared_ptr<Composite> composite = std::make_shared<Composite>(Composite{0}); /// all possiblt compositepointer  TODO turn to smartpointer
    std::map<std::string,std::shared_ptr<Shape>> shape_map; /// map with all the shapes accessible with their names


    std::map<std::string,std::shared_ptr<Material>> material_map;
    std::map<std::string,std::shared_ptr<Light>> light_map;
    std::map<std::string,std::shared_ptr<Camera>> camera_map;
    glm::vec3 ambient;

    std::string name_camera_render, filename;
    unsigned int x_res, y_res;


    while (std::getline(in_file, line_buffer)) {
        std::cout /*<< ++line_count*/ << line_buffer << std::endl;

        //construct stringstream using line_buffer string
        std::istringstream in_sstream(line_buffer);

        in_sstream >> identifier;

        //std::cout << "Identifier content: " << identifier << std::endl;

        //check for definition or transformation
        if ("define" == identifier) {
            std::string class_name;
            in_sstream >> class_name;
            //check for shapes/ materials/ lights
            if ("shape" == class_name) {
                std::string shape_type;
                in_sstream >> shape_type;
                //check for shape type, then parse attributes (including material lookup) - or composite
                if (shape_type == "box") {
                    //parse box attributes
                    std::string name_box, mat_name_box;
                    glm::vec3 p1, p2;
                    float p1_x, p1_y, p1_z;
                    float p2_x, p2_y, p2_z;

                    in_sstream >> name_box;
                    in_sstream >> p1_x >> p1_y >> p1_z;
                    in_sstream >> p2_x >> p2_y >> p2_z;
                    in_sstream >> mat_name_box;

                    p1[0] = p1_x;
                    p1[1] = p1_y;
                    p1[2] = p1_z;

                    p2[0] = p2_x;
                    p2[1] = p2_y;
                    p2[2] = p2_z;

                    // add a box and access it later with its name from the map
                    // set material to sphere (if its defined)
                    auto box = std::make_shared<Box>(Box{p1, p2});
                    auto i = material_map.find(mat_name_box);
                    if (i != material_map.end()) {
                        box->set_material(material_map[mat_name_box]);
                    } else {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }
                    shape_map.emplace(std::make_pair(name_box, box));

                } else if (shape_type == "sphere") {
                    //parse sphere attributes
                    std::string name_sphere, mat_name_sphere;
                    float radius;
                    float center_x, center_y, center_z;
                    glm::vec3 center;

                    in_sstream >> name_sphere;
                    in_sstream >> center_x >> center_y >> center_z;
                    in_sstream >> radius;
                    in_sstream >> mat_name_sphere;

                    center[0] = center_x;
                    center[1] = center_y;
                    center[2] = center_z;

                    // add a sphere and access it later with its name from the map
                    auto sphere = std::make_shared<Sphere>(Sphere{center, radius});

                    auto i = material_map.find(mat_name_sphere);
                    if (i != material_map.end()) {
                        sphere->set_material(material_map[mat_name_sphere]);
                    }
                    else {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }

                    shape_map.emplace(std::make_pair(name_sphere, sphere));

                } else if (shape_type == "triangle") {
                    //parse sphere attributes
                    std::string name_triangle, mat_name_triangle;
                    float x_1, y_1, z_1;
                    glm::vec3 vector_1;
                    float x_2, y_2, z_2;
                    glm::vec3 vector_2;
                    float x_3, y_3, z_3;
                    glm::vec3 vector_3;

                    in_sstream >> name_triangle;
                    in_sstream >> x_1 >> y_1 >> z_1;
                    in_sstream >> x_2 >> y_2 >> z_2;
                    in_sstream >> x_3 >> y_3 >> z_3;
                    in_sstream >> mat_name_triangle;

                    vector_1[0] = x_1;
                    vector_1[1] = y_1;
                    vector_1[2] = z_1;

                    vector_2[0] = x_2;
                    vector_2[1] = y_2;
                    vector_2[2] = z_2;

                    vector_3[0] = x_3;
                    vector_3[1] = y_3;
                    vector_3[2] = z_3;

                    // add a sphere and access it later with its name from the map
                    auto triangle = std::make_shared<Triangle>(Triangle{vector_1,vector_2,vector_3});

                    auto i = material_map.find(mat_name_triangle);
                    if (i != material_map.end()) {
                        triangle->set_material(material_map[mat_name_triangle]);
                    }
                    else {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }

                    shape_map.emplace(std::make_pair(name_triangle, triangle));

                }








                else if (shape_type == "composite") {
                    //parse composite attributes
                    //int count = 0;
                    std::string composite_name, param;
                    //std::vector <std::string> composites; //notwendig?

                    in_sstream >> composite_name;

                    std::cout << "Composite: " << composite_name<< std::endl;

                    std::vector<std::shared_ptr<Shape>> composite_shapes;

                    while (!in_sstream.eof()) {
                        in_sstream >> param;
                        //std::cout<<"Param: " <<param<< std::endl;
                        if (shape_map.find(param) != shape_map.end()) {
                            composite_shapes.push_back(shape_map[param]);
                        }
                    }

                    composite->add_shapes(composite_shapes);
                    //std::cout<< "Shapes loaded"<< std::endl;

                    // all the shapes have now been added to this single composite object, therefore we can build it now
                    composite->build();
                        //std::cout<< "composite build"<< std::endl;

                    // we will later test every ray against every composite object inside of this single vector
                    //composite->add_shape()push_back(composite);

                    // even shapes which are not inside a composite object might later be added to a new composite object to make the intersection tests faster
                }
            } else if ("material" == class_name) {
                //parse material attributes
                std::string material_name;
                float ka_red, ka_green, ka_blue;
                float kd_red, kd_green, kd_blue;
                float ks_red, ks_green, ks_blue;
                float m;

                glm::vec3 ka;
                glm::vec3 kd;
                glm::vec3 ks;

                in_sstream >> material_name;
                in_sstream >> ka_red >> ka_green >> ka_blue; //abmient reflection
                in_sstream >> kd_red >> kd_green >> kd_blue; //diffuse reflection
                in_sstream >> ks_red >> ks_green >> ks_blue; //reflecting reflection
                in_sstream >> m; //exponent for reflecting reflection


                ka[0] = ka_red;
                ka[1] = ka_green;
                ka[2] = ka_blue;

                kd[0] = kd_red;
                kd[1] = kd_green;
                kd[2] = kd_blue;

                ks[0] = ks_red;
                ks[1] = ks_green;
                ks[2] = ks_blue;



                auto material = std::make_shared<Material> (Material{ka, kd, ks, m});
                material_map.emplace(std::make_pair(material_name, material));

            } else if ("light" == class_name) {
                //parse light attributes
                std::string name_light;
                glm::vec3 pos, color, brightness;
                float pos_x, pos_y, pos_z, color_x, color_y, color_z, brightness_x, brightness_y, brightness_z;

                in_sstream >> name_light;
                in_sstream >> pos_x >> pos_y >> pos_z;
                in_sstream >> color_x >> color_y >> color_z;
                in_sstream >> brightness_x >> brightness_y >> brightness_z;

                pos[0] = pos_x;
                pos[1] = pos_y;
                pos[2] = pos_z;

                color[0] = color_x;
                color[1] = color_y;
                color[2] = color_z;

                brightness[0] = brightness_x;
                brightness[1] = brightness_y;
                brightness[2] = brightness_z;

                auto light = std::make_shared<Light> (Light(pos, color, brightness));
                light_map.emplace(std::make_pair(name_light, light));

            } else if ("camera" == class_name) {
                //parse camera attributes
                std::string name_camera;
                float fov_x;

                in_sstream >> name_camera;
                in_sstream >> fov_x;

                auto camera = std::make_shared<Camera> (Camera(fov_x));
                camera_map.emplace(std::make_pair(name_camera, camera));

            } else {
                std::cout << "Line was not valid!" << std::endl;
            }
        } else if ("transform" == identifier) {
            std::string object, transfomation_type;
            in_sstream >> object;
            in_sstream >> transfomation_type;
            //check for transformation type and parse arguments
            if ("translate" == transfomation_type) {
                glm::vec3 offset;
                float offset_x, offset_y, offset_z;

                in_sstream >> offset_x >> offset_y >> offset_z;

                offset[0] = offset_x;
                offset[1] = offset_y;
                offset[2] = offset_z;

            } else if ("rotate" == transfomation_type) {
                glm::vec3 vector;
                float angle;
                float vector_x, vector_y, vector_z;

                in_sstream >> angle;
                in_sstream >> vector_x >> vector_y >> vector_z;

                vector[0] = vector_x;
                vector[1] = vector_y;
                vector[2] = vector_z;

            } else if ("scale" == transfomation_type) {
                float value;

                in_sstream >> value;

            } else {
                std::cout << "Line was not valid!" << std::endl;
            }
        } else if ("ambient" == identifier) {
            //parse ambient attributes
            float ambient_x, ambient_y, ambient_z;

            in_sstream >> ambient_x >> ambient_y >> ambient_z;

            ambient[0] = ambient_x;
            ambient[1] = ambient_y;
            ambient[2] = ambient_z;

        } else if ("render" == identifier) {

            in_sstream >> name_camera_render >> filename >> x_res >> y_res;

            //create scene with root of composite tree
            Scene scene (composite, light_map, camera_map, ambient);

            auto i = camera_map.find(name_camera_render);
            if (i != camera_map.end()) {
                 scene.draw_scene(*camera_map[name_camera_render], filename, x_res, y_res);
            } else {
                std::cout << "Please only render scene with defined camera!" << std::endl;
            }


        } else if ("#" == identifier) {
            //commentary - do nothing
        } else {
            std::cout << "Line was not valid!" << std::endl;
        }
    }


    //close file
    in_file.close();
}
