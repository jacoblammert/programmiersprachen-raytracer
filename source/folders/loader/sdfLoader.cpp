#include "sdfLoader.hpp"

SdfLoader::SdfLoader(std::string filepath) :
        filepath_{std::move(filepath)} {}

void SdfLoader::load_file() { //const correctness valid?

    if (filepath_.empty()) {
        std::cout << "Please set a valid filepath" << std::endl;
        return;
    }

    //open file in read-only && ASCII mode
    std::ifstream in_file(filepath_, std::ios::in);
    std::string line_buffer;

    std::string identifier;


    std::shared_ptr<Composite> composite = std::make_shared<Composite>(
            Composite{0}); /// all possiblt compositepointer  TODO turn to smartpointer
    std::vector<std::shared_ptr<Shape>> shapes; /// map with all the shapes accessible with their names


    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Camera>> cameras;
    glm::vec3 ambient;

    std::string name_camera_render, filename;
    unsigned int x_res, y_res;


    while (std::getline(in_file, line_buffer)) {
        std::cout << line_buffer << std::endl;

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
                    glm::vec3 p1, p2, axis;
                    float p1_x = 0.0f, p1_y = 0.0f, p1_z = 0.0f;
                    float p2_x = 0.0f, p2_y = 0.0f, p2_z = 0.0f;
                    float a_x = 0.0f, a_y = 1.0f, a_z = 0.0f;

                    in_sstream >> name_box;
                    in_sstream >> p1_x >> p1_y >> p1_z;
                    in_sstream >> p2_x >> p2_y >> p2_z;
                    in_sstream >> a_x >> a_y >> a_z;
                    in_sstream >> mat_name_box;

                    p1[0] = p1_x;
                    p1[1] = p1_y;
                    p1[2] = p1_z;

                    p2[0] = p2_x;
                    p2[1] = p2_y;
                    p2[2] = p2_z;

                    axis[0] = a_x;
                    axis[1] = a_y;
                    axis[2] = a_z;

                    // add a box and access it later with its name from the map
                    // set material to sphere (if its defined)
                    auto box = std::make_shared<Box>(Box{name_box, p1, p2});
                    box->set_rotation_axis(axis);
                    bool material_found = false;

                    for (auto const &i : materials) {
                        if (i->name == mat_name_box) {
                            box->set_material(i);
                            material_found = true;
                        }
                    }
                    if (!material_found) {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }
                    shapes.push_back(box);

                } else if (shape_type == "sphere") {
                    //parse sphere attributes
                    std::string name_sphere, mat_name_sphere;
                    float radius = 0.0f;
                    float center_x = 0.0f, center_y = 0.0f, center_z = 0.0f;
                    glm::vec3 center;

                    in_sstream >> name_sphere;
                    in_sstream >> center_x >> center_y >> center_z;
                    in_sstream >> radius;
                    in_sstream >> mat_name_sphere;

                    center[0] = center_x;
                    center[1] = center_y;
                    center[2] = center_z;

                    // add a sphere and access it later with its name from the map
                    auto sphere = std::make_shared<Sphere>(Sphere{name_sphere, center, radius});
                    bool material_found = false;

                    for (auto const &i : materials) {
                        if (i->name == mat_name_sphere) {
                            sphere->set_material(i);
                            material_found = true;
                        }
                    }
                    if (!material_found) {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }

                    shapes.push_back(sphere);

                } else if (shape_type == "triangle") {
                    //parse triangle attributes - define shape triangle [vec1] [vec2] [vec3] <mat-name>
                    std::string name_triangle, mat_name_triangle;
                    float x_1 = 0.0f, y_1 = 0.0f, z_1 = 0.0f;
                    glm::vec3 vector_1;
                    float x_2 = 0.0f, y_2 = 0.0f, z_2 = 0.0f;
                    glm::vec3 vector_2;
                    float x_3 = 0.0f, y_3 = 0.0f, z_3 = 0.0f;
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

                    // add a triangle and access it later with its name from the map
                    auto triangle = std::make_shared<Triangle>(Triangle{name_triangle, vector_1, vector_2, vector_3});
                    bool material_found = false;

                    for (auto const &i : materials) {
                        if (i->name == mat_name_triangle) {
                            triangle->set_material(i);
                            material_found = true;
                        }
                    }
                    if (!material_found) {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }

                    shapes.push_back(triangle);

                } else if (shape_type == "cone" || shape_type == "cylinder") {
                    //parse cone/cylinder attributes - define shape cone/cylinder [pos] [axis] <width> <height> <mat-name>
                    std::string name_cone_cylinder, mat_name_cone_cylinder;
                    float position_x = 0.0f, position_y = 0.0f, position_z = 0.0f;
                    float axis_x = 0.0f, axis_y = 0.0f, axis_z = 0.0f;
                    float width = 0.0f, height = 0.0f;
                    glm::vec3 position, axis;

                    in_sstream >> name_cone_cylinder;
                    in_sstream >> position_x >> position_y >> position_z;
                    in_sstream >> axis_x >> axis_y >> axis_z;
                    in_sstream >> width >> height;
                    in_sstream >> mat_name_cone_cylinder;

                    position[0] = position_x;
                    position[1] = position_y;
                    position[2] = position_z;

                    axis[0] = axis_x;
                    axis[1] = axis_y;
                    axis[2] = axis_z;

                    if (shape_type == "cone") {
                        // add a cone and access it later with its name from the map
                        auto cone = std::make_shared<Cone>(Cone{name_cone_cylinder, position, axis, width, height});
                        bool material_found = false;

                        for (auto const &i : materials) {
                            if (i->name == mat_name_cone_cylinder) {
                                cone->set_material(i);
                                material_found = true;
                            }
                        }
                        if (!material_found) {
                            std::cout << "Please only use defined materials!" << std::endl;
                        }
                        shapes.push_back(cone);

                    } else {
                        // add a cylinder and access it later with its name from the map
                        auto cylinder = std::make_shared<Cylinder>(
                                Cylinder{name_cone_cylinder, position, axis, width, height});
                        bool material_found = false;

                        for (auto const &i : materials) {
                            if (i->name == mat_name_cone_cylinder) {
                                cylinder->set_material(i);
                                material_found = true;
                            }
                        }
                        if (!material_found) {
                            std::cout << "Please only use defined materials!" << std::endl;
                        }

                        shapes.push_back(cylinder);
                    }


                } else if (shape_type == "plane") {
                    //parse plane attributes - define shape plane [pos] [normal] <mat-name>
                    std::string name_plane, mat_name_plane;
                    float position_x = 0.0f, position_y = 0.0f, position_z = 0.0f;
                    float normal_x = 0.0f, normal_y = 0.0f, normal_z = 0.0f;
                    glm::vec3 position, normal;

                    in_sstream >> name_plane;
                    in_sstream >> position_x >> position_y >> position_z;
                    in_sstream >> normal_x >> normal_y >> normal_z;
                    in_sstream >> mat_name_plane;

                    position[0] = position_x;
                    position[1] = position_y;
                    position[2] = position_z;

                    normal[0] = normal_x;
                    normal[1] = normal_y;
                    normal[2] = normal_z;

                    // add a cone and access it later with its name from the map
                    auto plane = std::make_shared<Plane>(Plane{name_plane, position, normal});

                    bool material_found = false;

                    for (auto const &i : materials) {
                        if (i->name == mat_name_plane) {
                            plane->set_material(i);
                            material_found = true;
                        }
                    }
                    if (!material_found) {
                        std::cout << "Please only use defined materials!" << std::endl;
                    }
                    shapes.push_back(plane);

                } else if (shape_type == "composite") {
                    //parse composite attributes
                    //int count = 0;
                    std::string composite_name, param;
                    //std::vector <std::string> composites; //notwendig?

                    in_sstream >> composite_name;

                    composite->set_name(composite_name);

                    std::vector<std::shared_ptr<Shape>> composite_shapes;

                    while (!in_sstream.eof()) {

                        in_sstream >> param;
                        std::cout << "######################Param: " << param << std::endl;

                        for (auto const &i : shapes) {
                            if (i->get_name() == param) {
                                composite_shapes.push_back(i);
                                param = ""; /// important! Since we added the shape to the composite vector, we need to change the param to avoid a false positive
                            }
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
                float ka_red = 0.0f, ka_green = 0.0f, ka_blue = 0.0f;
                float kd_red = 0.0f, kd_green = 0.0f, kd_blue = 0.0f;
                float ks_red = 0.0f, ks_green = 0.0f, ks_blue = 0.0f;
                float m = 0.0f;
                float opacity = 1.0f;
                float reflectivity = 0.0f;
                float refractive_index = 1.0f;
                float roughness = 0.0f;

                glm::vec3 ka;
                glm::vec3 kd;
                glm::vec3 ks;

                in_sstream >> material_name;
                in_sstream >> ka_red >> ka_green >> ka_blue; //ambient reflection
                in_sstream >> kd_red >> kd_green >> kd_blue; //diffuse reflection
                in_sstream >> ks_red >> ks_green >> ks_blue; //reflecting reflection
                in_sstream >> m; //exponent for reflecting reflection
                in_sstream >> opacity; //opacity
                in_sstream >> reflectivity; //reflectivity
                in_sstream >> refractive_index; //refractive index
                in_sstream >> roughness; //refractive index


                ka[0] = ka_red;
                ka[1] = ka_green;
                ka[2] = ka_blue;

                kd[0] = kd_red;
                kd[1] = kd_green;
                kd[2] = kd_blue;

                ks[0] = ks_red;
                ks[1] = ks_green;
                ks[2] = ks_blue;


                auto material = std::make_shared<Material>(Material{material_name, ka, kd, ks, m});

                material->opacity = opacity;
                material->glossy = reflectivity;
                material->refractive_index = refractive_index;
                material->roughness = roughness;

                materials.push_back(material);

            } else if ("light" == class_name) {
                //parse light attributes
                std::string name_light;
                glm::vec3 pos, color, brightness;
                float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
                float color_x = 0.0f, color_y = 0.0f, color_z = 0.0f;
                float brightness_x = 0.0f, brightness_y = 0.0f, brightness_z = 0.0f;

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

                auto light = std::make_shared<Light>(Light(name_light, pos, color, brightness));
                lights.push_back(light);

            } else if ("camera" == class_name) {
                //parse camera attributes
                std::string name_camera;
                float fov_x = 0.0f;
                glm::vec3 eye, direction, up;
                char eye_x_test = '\0';
                float eye_x = 0.0f, eye_y = 0.0f, eye_z = 0.0f;
                float direction_x = 0.0f, direction_y = 0.0f, direction_z = 0.0f;
                float up_x = 0.0f, up_y = 0.0f, up_z = 0.0f;

                in_sstream >> name_camera;
                in_sstream >> fov_x;
                in_sstream >> eye_x;

                //if (eye_x != '\0') {
                in_sstream >> eye_y >> eye_z >> direction_x >> direction_y >> direction_z >> up_x >> up_y >> up_z;

                eye[0] = eye_x;
                eye[1] = eye_y;
                eye[2] = eye_z;

                direction[0] = direction_x;
                direction[1] = direction_y;
                direction[2] = direction_z;

                up[0] = up_x;
                up[1] = up_y;
                up[2] = up_z;

                std::cout << eye[0] << eye[1] << eye[2] << std::endl;
                std::cout << direction[0] << direction[1] << direction[2] << std::endl;
                std::cout << up[0] << up[1] << up[2] << std::endl;

                auto camera = std::make_shared<Camera>(Camera(name_camera, fov_x, eye, direction, up));
                cameras.push_back(camera);

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
                float offset_x = 0.0f, offset_y = 0.0f, offset_z = 0.0f;

                in_sstream >> offset_x >> offset_y >> offset_z;

                offset[0] = offset_x;
                offset[1] = offset_y;
                offset[2] = offset_z;

            } else if ("rotate" == transfomation_type) {
                glm::vec3 vector;
                float angle = 0.0f;
                float vector_x = 0.0f, vector_y = 0.0f, vector_z = 0.0f;

                in_sstream >> angle;
                in_sstream >> vector_x >> vector_y >> vector_z;

                vector[0] = vector_x;
                vector[1] = vector_y;
                vector[2] = vector_z;

            } else if ("scale" == transfomation_type) {
                float value = 0.0f;

                in_sstream >> value;

            } else {
                std::cout << "Line was not valid!" << std::endl;
            }
        } else if ("ambient" == identifier) {
            //parse ambient attributes
            float ambient_x = 0.0f, ambient_y = 0.0f, ambient_z = 0.0f;

            in_sstream >> ambient_x >> ambient_y >> ambient_z;

            ambient[0] = ambient_x;
            ambient[1] = ambient_y;
            ambient[2] = ambient_z;

        } else if ("render" == identifier) {

            in_sstream >> name_camera_render >> filename >> x_res >> y_res;

            //create scene with root of composite tree
            scene_ = std::make_shared<Scene>(
                    Scene{materials, composite, lights, cameras, ambient, (int) x_res, (int) y_res});


            bool camera_found = false;

            for (auto const &i : cameras) {
                if (i->get_name() == name_camera_render) {
                    camera_found = true;
                }
            }
            if (!camera_found) {
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

std::shared_ptr<Scene> SdfLoader::get_Scene() {
    return scene_;
}
