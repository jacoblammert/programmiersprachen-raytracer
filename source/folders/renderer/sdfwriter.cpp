//#include <omp.h>
#include "scene.hpp"
#include <algorithm>
SdfWriter::SdfWriter(std::string const& file) :
    file_ {file}
{}

void SdfWriter::create_sdf (std::vector<std::shared_ptr<Material>> materials,
                            std::shared_ptr<Composite> composite,
                            std::vector<std::shared_ptr<Light>> lights_,
                            std::vector<std::shared_ptr<Camera>> cameras_,
                            glm::vec3 ambient_) const {
    
    std::fstream file(file_.c_str(), std::ios::out);
    file.clear();
    
    std::vector<std::shared_ptr<Shape>> shapes = composite->get_shapes();
    //std::vector<std::shared_ptr<Material>> materials;

    file << "# materials car cag cab cdr cdg cdb csr csg csb reflective_exponent opacity reflectivity refractive_index roughness\n";
    
    for (auto const& material : materials) {
        //auto material = i->get_material();
        //look if material is already in vector materials - if false add material
        //if (std::find(materials.begin(), materials.end(), material) == materials.end()) {
            //materials.push_back(material);
            //put material in file as definition

            file << "define material " << material->name_ << " "
            << material->color_ambient_[0] << " " << material->color_ambient_[1] << " " << material->color_ambient_[2] << " "
            << material->color_diffuse_[0] << " " << material->color_diffuse_[1] << " " << material->color_diffuse_[2] << " "
            << material->color_specular_[0] << " " << material->color_specular_[1] << " " << material->color_specular_[2] << " "
            << material->reflective_exponent_ << " "
            << material->opacity_ << " "
            << material->glossy_ << " "
            << material->refractive_index_ << " "
            << material->roughness_
            << "\n";
        //}
    }
    file<<"# geometry\n";
    
    for (auto const& i : shapes) {
        switch (i->get_shape_type()) {
            case BOX:
                file << "define shape box "<<i->get_information();
                file << "\n";
                break;
            case CONE:
                file << "define shape cone " <<i->get_information();
                file << "\n";
                break;
            case CYLINDER:
                file << "define shape cylinder " <<i->get_information();
                file << "\n";
                break;
            case PLANE:
                file << "define shape plane " <<i->get_information();
                file << "\n";
                break;
            case SPHERE:
                file << "define shape sphere " <<i->get_information();
                file << "\n";
                break;
            case TRIANGLE:
                file << "define shape triangle " <<i->get_information();
                file << "\n";
                break;
            default:
                break;
        }
    }
    file<<"# composite\n";

    file << "define shape composite " <<composite->get_information();
    file << "\n";

    //TODO add light,...


    file <<"define light sun 500 800 0 1.0 1.0 1.0 100 100 100\nambient 0.1 0.1 0.1\ndefine camera eye 60.0\ntransform eye rotate -45 0 1 0\ntransform eye translate 100 0 100\nrender eye image.ppm 480 320";
    
}
