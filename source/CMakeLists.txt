add_executable(example example.cpp folders/shapes/shape.cpp folders/shapes/shape.h folders/camera/ray.h folders/shapes/sphere.cpp folders/shapes/sphere.h folders/camera/camera.cpp folders/camera/camera.h folders/shapes/plane.cpp folders/shapes/plane.h folders/shapes/box.cpp folders/shapes/box.h folders/shapes/triangle.cpp folders/shapes/triangle.h)

target_link_libraries(example ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(example glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS example DESTINATION .)

add_executable(raytracer raytracer.cpp folders/loader/sdfLoader.cpp folders/shapes/shape.cpp folders/shapes/shape.h folders/camera/ray.h folders/shapes/sphere.cpp folders/shapes/sphere.h folders/camera/camera.cpp folders/camera/camera.h folders/shapes/plane.cpp folders/shapes/plane.h folders/shapes/box.cpp folders/shapes/box.h folders/shapes/triangle.cpp folders/shapes/triangle.h)

target_link_libraries(raytracer ${FRAMEWORK_NAME} ${LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
  )
add_dependencies(raytracer glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS raytracer DESTINATION .)

# Build tests
add_executable(tests
        ../framework/prog5/tests.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Sphere.cpp ../framework/prog5/Box.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Ray.cpp ../framework/prog5/HitPoint.cpp)
target_link_libraries(tests
  ${GLFW_LIBRARIES}
  )

