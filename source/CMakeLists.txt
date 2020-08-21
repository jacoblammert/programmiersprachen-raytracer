

add_executable(example example.cpp  folders/camera/ray.cpp folders/camera/ray.hpp folders/shapes/shape.cpp folders/shapes/shape.hpp folders/shapes/sphere.cpp folders/shapes/sphere.hpp folders/camera/camera.cpp folders/camera/camera.hpp folders/shapes/plane.cpp folders/shapes/plane.hpp folders/shapes/box.cpp folders/shapes/box.hpp folders/shapes/triangle.cpp folders/shapes/triangle.hpp folders/camera/ray.cpp folders/shapes/composite.cpp folders/shapes/composite.hpp)

target_link_libraries(example ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(example glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS example DESTINATION .)

add_executable(raytracer raytracer.cpp folders/loader/sdfLoader.cpp folders/shapes/shape.cpp folders/shapes/shape.hpp folders/camera/ray.cpp folders/camera/ray.hpp folders/shapes/sphere.cpp folders/shapes/sphere.hpp folders/camera/camera.cpp folders/camera/camera.hpp folders/shapes/plane.cpp folders/shapes/plane.hpp folders/shapes/box.cpp folders/shapes/box.hpp folders/shapes/triangle.cpp folders/shapes/triangle.hpp folders/camera/ray.cpp folders/shapes/composite.cpp folders/shapes/composite.hpp)

#set(CMAKE_CXX_STANDARD 14)
#find_package(OpenMP REQUIRED)

##if(OpenMP_CXX_FOUND)
##    target_link_libraries(raytracer PUBLIC OpenMP::OpenMP_CXX)
##endif()


target_link_libraries(raytracer ${FRAMEWORK_NAME} ${LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
  #OpenMP::OpenMP_CXX
  )
add_dependencies(raytracer glfw ${FRAMEWORK_NAME} ${COPY_BINARY})





install(TARGETS raytracer DESTINATION .)

# Build tests
add_executable(tests
        ../framework/prog5/tests.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Sphere.cpp ../framework/prog5/Box.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Ray.cpp ../framework/prog5/HitPoint.cpp)
target_link_libraries(tests
  ${GLFW_LIBRARIES}
  )




