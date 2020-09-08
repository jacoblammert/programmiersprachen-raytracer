#include <algorithm>
#include "sdfwriter.hpp"
#include <utility>

SdfWriter::SdfWriter(std::string file) :
        filename_{std::move(file)} {
    file_ = "../../source/folders/sdfFiles/" + filename_;
}

void SdfWriter::create_sdf(const std::shared_ptr<Scene> &scene) {

    std::vector<std::shared_ptr<Material>> materials = scene->materials_;
    std::shared_ptr<Composite> composite = scene->composite_;
    std::vector<std::shared_ptr<Light>> lights_ = scene->lights_;
    std::vector<std::shared_ptr<Camera>> cameras_ = scene->cameras_;
    glm::vec3 ambient_ = scene->ambient_;


    std::fstream file(file_.c_str(), std::ios::out);
    file.clear();

    std::vector<std::shared_ptr<Shape>> shapes = composite->get_shapes();
    //std::vector<std::shared_ptr<Material>> materials;

    file
            << "# materials car cag cab cdr cdg cdb csr csg csb reflective_exponent opacity reflectivity refractive_index roughness\n";

    for (auto const &material : materials) {
        //auto material = i->get_material();
        //look if material is already in vector materials - if false add material
        //if (std::find(materials.begin(), materials.end(), material) == materials.end()) {
        //materials.push_back(material);
        //put material in file as definition

        file << "define material " << material->name << " "
             << material->color_ambient[0] << " " << material->color_ambient[1] << " " << material->color_ambient[2]
             << " "
             << material->color_diffuse[0] << " " << material->color_diffuse[1] << " " << material->color_diffuse[2]
             << " "
             << material->color_specular[0] << " " << material->color_specular[1] << " "
             << material->color_specular[2] << " "
             << material->reflective_exponent << " "
             << material->opacity << " "
             << material->glossy << " "
             << material->refractive_index << " "
             << material->roughness
             << "\n";
        //}
    }
    file << "# geometry\n";

    for (auto const &i : shapes) {
        switch (i->get_shape_type()) {
            case BOX:
                file << "define shape box " << i->get_information();
                file << "\n";
                break;
            case CONE:
                file << "define shape cone " << i->get_information();
                file << "\n";
                break;
            case CYLINDER:
                file << "define shape cylinder " << i->get_information();
                file << "\n";
                break;
            case PLANE:
                file << "define shape plane " << i->get_information();
                file << "\n";
                break;
            case SPHERE:
                file << "define shape sphere " << i->get_information();
                file << "\n";
                break;
            case TRIANGLE:
                file << "define shape triangle " << i->get_information();
                file << "\n";
                break;
            default:
                break;
        }
    }
    file << "# composite\n";

    file << "define shape composite " << composite->get_information();
    file << "\n";

    //file <<"define light sun 500 800 0 1.0 1.0 1.0 100 100 100\nambient 0.1 0.1 0.1\ndefine camera eye 60.0\ntransform eye rotate -45 0 1 0\ntransform eye translate 100 0 100\nrender eye image.ppm 480 320";

    for (auto const &light : lights_) {
        file << "define light " << light->name << " " << light->position[0] << " " << light->position[1] << " "
             << light->position[2] << " " << light->color[0] << " " << light->color[1] << " " << light->color[2] << " "
             << light->brightness[0] << " " << light->brightness[1] << " " << light->brightness[2] << "\n";
    }

    file << "ambient " << std::to_string(scene->ambient_[0]) << " " << std::to_string(scene->ambient_[1]) << " "
         << std::to_string(scene->ambient_[2]) << "\n";

    for (auto const &camera : cameras_) {
        file << "define camera" << camera->get_information() << "\n";
    }

    //transformations

    //welche Kamera nehmen, doch nur eine sinnvoll?
    file << "render " << cameras_[0]->get_name() << " " << filename_ << " " << scene->x_res_ << " " << scene->y_res_
         << "\n";


}
