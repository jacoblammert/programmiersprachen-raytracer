
#include <memory>
#include "folders/loader/sdfLoader.hpp"
#include "folders/loader/sdfwriter.hpp"
#include "folders/loader/objLoader.hpp"


int main(int argc, char *argv[]) {


    int frames = 200;//24 * 10; // 24 frames per second -> 10s long Animation

    std::string animation_name = "final animation";


    /// Creates all the sdf files needed
    /// If we want to change anything in the animation (add a material/ Shape/ Change the resolution) we can do it in the original (start.sdf) file
    //SdfLoader loader{"../../source/folders/sdfFiles/" + animation_name + std::to_string(999) + ".sdf"};
    SdfLoader loader{"../../source/folders/sdfFiles/Animation_1.sdf"};

    loader.load_file();
    
    std::shared_ptr<Scene> scene = loader.get_scene();


// Loading objs
    // The following lines load an obj file (without comments)
/**/
    objLoader obj_loader;
    // "Deletes all the shapes from the scene and only uses the new obj shapes
    std::vector<std::shared_ptr<Shape>> all_shapes = scene->composite_->get_shapes();

    // or "../../source/folders/objFiles/cube.obj"
    // or "../../source/folders/objFiles/car.obj"
    // or "../../source/folders/objFiles/dragon_large.obj" -> unstable, crashes when moving the camera

    std::vector<std::shared_ptr<Shape>> loaded_shapes = obj_loader.load_obj("../../source/folders/objFiles/statue_cpp_raytracer.obj");

    std::shared_ptr<Material> obj_material = std::make_shared<Material>(Material("obj_material",{0.3,1,0.3},{0.7,1,0.7},{1,1,1},10));

    //obj_material->opacity = 0;
    //obj_material->refractive_index = 1.36;

    for (int i = 0; i < loaded_shapes.size(); ++i) {
        loaded_shapes[i]->set_material(obj_material);
    }

    std::cout<<"Obj Loaded size: "<<loaded_shapes.size()<<std::endl;

    all_shapes.insert(all_shapes.end(),loaded_shapes.begin(),loaded_shapes.end());
    scene->composite_ = std::make_shared<Composite>(Composite(scene->composite_->get_name(),all_shapes));
    scene->materials_.push_back(obj_material);
    scene->antialiasing_samples_ = 1;
//*/



// Animation:


/*/
    for (int i = 0; i < frames; ++i) {

        float progress = (float)i/(float)frames;

        //progress = 0.30f;

        float camera_distance = 10;
        scene->cameras_[0]->set_position(glm::vec3{camera_distance * sin(progress * 2 * MATH_PI),6,camera_distance * cos(progress * 2 * MATH_PI)}); // The Camera rotates around the origin with a height of 4
        scene->cameras_[0]->look_at(glm::vec3{0,4,0}); /// The Camera looks at the origin

        std::cout<<"Rendering\n";

        /// Example of editing the scenes content. The reflectivity varies from 0 to 1
        //obj_material->glossy = sin(progress * 2 * MATH_PI);
        //obj_material->roughness = 0.05;


        SdfWriter sdf_writer(animation_name + std::to_string(i) + ".sdf");
        sdf_writer.create_sdf(scene);
    }/**/


/*/
    /// loads all the written sdf files and renders the images
    for (int i = 0; i < frames; ++i) {


        loader = {"../../source/folders/sdfFiles/" + animation_name + std::to_string(i) + ".sdf"};
        loader.load_file();

        scene = loader.get_scene();
        //std::vector<std::shared_ptr<Shape>> all_shapes = scene->composite_->get_shapes();

        scene->antialiasing_samples_ = 1;

        scene->draw_frame(0,"../../source/folders/images/"+animation_name + std::to_string(i) + ".ppm"); /// Renders the loaded sdf file and saves it as ppm
        std::cout<<"Saved image: " + std::to_string(i)<<std::endl;
    }/**/





    scene->draw_scene(0, "image.ppm"); // Maybe open a window after it rendered? With the information of the last .sdf file


 
    return 0;
}
