//#include <omp.h>
#include "scene.hpp"

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
            file << "define material " << material->name << " " << material->color_ambient_[0] << " " << material->color_ambient_[1] << " " << material->color_ambient_[2] << " " << material->color_diffuse_[0] << " " << material->color_diffuse_[1] << " " << material->color_diffuse_[2] << " " << material->color_specular_[0] << " " << material->color_specular_[1] << " " << material->color_specular_[2] << " " << material->reflective_exponent_ << "\n";
        }
    }
    
    for (auto const& i : shapes) {
        switch (i->get_shape_type()) {
            case BOX:
                //
            case CONE:
                //
            case CYLINDER:
                //
            case PLANE:
                //
            case SPHERE:
                //
            case TRIANGLE:
                //
            case COMPOSITE:
                //
            default:
                break;
        }
    }
    
    
}
