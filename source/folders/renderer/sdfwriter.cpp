//#include <omp.h>
#include "scene.hpp"
#include <algorithm>
SdfWriter::SdfWriter(std::string const& file) :
    file_ {file}
{}

void SdfWriter::create_sdf (std::shared_ptr<Composite> composite,
                            std::vector<std::shared_ptr<Light>> lights_,
                            std::vector<std::shared_ptr<Camera>> cameras_,
                            glm::vec3 ambient_) const {
    
    std::fstream file(file_.c_str(), std::ios::out);
    file.clear();
    
    std::vector<std::shared_ptr<Shape>> shapes = composite->get_shapes();
    std::vector<std::shared_ptr<Material>> materials;
    
    
    for (auto const& i : shapes) {
        auto material = i->get_material();
        //look if material is already in vector materials - if false add material
        if (std::find(materials.begin(), materials.end(), material) == materials.end()) {
            materials.push_back(material);
            //put material in file as definition
            file << "define material " << material->name_ << " " << material->color_ambient_[0] << " " << material->color_ambient_[1] << " " << material->color_ambient_[2] << " " << material->color_diffuse_[0] << " " << material->color_diffuse_[1] << " " << material->color_diffuse_[2] << " " << material->color_specular_[0] << " " << material->color_specular_[1] << " " << material->color_specular_[2] << " " << material->reflective_exponent_ << "\n";
        }
    }
    
    for (auto const& i : shapes) {
        int count_box = 0, count_cone = 0, count_cylinder = 0, count_plane = 0, count_sphere = 0, count_triangle = 0, count_composite = 0;
        switch (i->get_shape_type()) {
            case BOX:
                file << "define shape box "<<i->get_information();
                file << "\n";
                i->print(file);
                count_box++;
                break;
            case CONE:
                file << "define shape cone " <<i->get_information();
                file << "\n";
                count_cone++;
                break;
            case CYLINDER:
                file << "define shape cylinder " <<i->get_information();
                file << "\n";
                count_cylinder++;
                break;
            case PLANE:
                file << "define shape plane " <<i->get_information();
                file << "\n";
                count_plane++;
                break;
            case SPHERE:
                file << "define shape sphere " <<i->get_information();
                file << "\n";
                count_sphere++;
                break;
            case TRIANGLE:
                file << "define shape triangle " <<i->get_information();
                file << "\n";
                count_triangle++;
                break;
            case COMPOSITE:
                file << "define shape composite composite" << count_composite<< " " <<i->get_information();
                file << "\n";
                count_composite++;
                break;
            default:
                break;
        }
    }
    
    
}
