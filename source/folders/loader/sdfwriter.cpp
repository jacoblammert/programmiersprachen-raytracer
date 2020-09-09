#include "sdfwriter.hpp"

/**
 Constructor for creating a sdfwriter
 @param file place where the sdf is saved
 */
SdfWriter::SdfWriter (std::string const& file) :
        filename_{file}
{
    file_ = "../../source/folders/sdfFiles/" + filename_;
}

/**
 Creates a sdf file for the given scene
 @param scene the sdf file is created for
 */
void SdfWriter::create_sdf (std::shared_ptr<Scene> const& scene) {

    // materials used for objects
    std::vector<std::shared_ptr<Material>> materials = scene->materials_;
    // composite where objects are stored
    std::shared_ptr<Composite> composite = scene->composite_;
    // lights of the scene
    std::vector<std::shared_ptr<Light>> lights_ = scene->lights_;
    // cameras of the scene
    std::vector<std::shared_ptr<Camera>> cameras_ = scene->cameras_;
    // ambient light of the scene
    glm::vec3 ambient_ = scene->ambient_;
    // getting shapes of scene through composite
    std::vector<std::shared_ptr<Shape>> shapes = composite->get_shapes();
    
    std::fstream file (file_.c_str(), std::ios::out);
    file.clear();

    // write/ define materials in sdf file with the material attributes
    file << "# materials car cag cab cdr cdg cdb csr csg csb reflective_exponent opacity reflectivity refractive_index roughness\n";
    
    for (auto const& material : materials) {

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
    }
    
    // write/ define shapes in sdf file according to shape type and write the attributes of the shape
    file << "# geometry\n";

    for (auto const& i : shapes) {
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
    
    // write information of composite (which elements are within composite)
    file << "# composite\n";
    file << "define shape composite " << composite->get_information() << "\n";
    
    // write/ define lights in sdf file
    file << "# lights\n";
    
    for (auto const& light : lights_) {
        file << "define light " << light->name << " " << light->position[0] << " " << light->position[1] << " "
             << light->position[2] << " " << light->color[0] << " " << light->color[1] << " " << light->color[2] << " "
             << light->brightness[0] << " " << light->brightness[1] << " " << light->brightness[2] << "\n";
    }

    // write/ define ambient light in sdf file
    file << "# ambient\n";
    file << "ambient " << std::to_string(scene->ambient_[0]) << " " << std::to_string(scene->ambient_[1]) << " "
         << std::to_string(scene->ambient_[2]) << "\n";

    // write/ define ambient cameras in sdf file
    file << "# cameras\n";
    for (auto const& camera : cameras_) {
        file << "define camera" << camera->get_information();
    }

    // TODO transformationen nicht direkt mit übergeben sondern trennen wie in sdf beispiel

    // TODO welche Kamera nehmen? - einfach erste?
    file << "render " << cameras_[0]->get_name() << " " << filename_ << " " << scene->x_res_ << " " << scene->y_res_
         << "\n";


}
