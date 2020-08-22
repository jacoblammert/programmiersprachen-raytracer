

add_executable(example example.cpp  folders/camera/ray.cpp folders/camera/ray.hpp folders/shapes/shape.cpp folders/shapes/shape.hpp folders/shapes/sphere.cpp folders/shapes/sphere.hpp folders/camera/camera.cpp folders/camera/camera.hpp folders/shapes/plane.cpp folders/shapes/plane.hpp folders/shapes/box.cpp folders/shapes/box.hpp folders/shapes/triangle.cpp folders/shapes/triangle.hpp folders/camera/ray.cpp folders/shapes/composite.cpp folders/shapes/composite.hpp folders/renderer/render.cpp folders/renderer/render.hpp folders/renderer/light.cpp folders/renderer/light.hpp)

target_link_libraries(example ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(example glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS example DESTINATION .)

add_executable(raytracer raytracer.cpp folders/loader/sdfLoader.cpp folders/shapes/shape.cpp folders/shapes/shape.hpp folders/camera/ray.cpp folders/camera/ray.hpp folders/shapes/sphere.cpp folders/shapes/sphere.hpp folders/camera/camera.cpp folders/camera/camera.hpp folders/shapes/plane.cpp folders/shapes/plane.hpp folders/shapes/box.cpp folders/shapes/box.hpp folders/shapes/triangle.cpp folders/shapes/triangle.hpp folders/camera/ray.cpp folders/shapes/composite.cpp folders/shapes/composite.hpp folders/renderer/render.cpp folders/renderer/render.hpp)

set(CMAKE_EXE_LINKER_FLAGS "-static")#remove if it does not work
set(CMAKE_CXX_STANDARD 14)#remove for non omp
find_package(OpenMP REQUIRED)#remove for non omp



target_link_libraries(raytracer ${FRAMEWORK_NAME} ${LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
        OpenMP::OpenMP_CXX #remove for non omp
  )
add_dependencies(raytracer glfw ${FRAMEWORK_NAME} ${COPY_BINARY})


#set(SOURCE_FILES raytracer.cpp)
#add_executable(untitled ${SOURCE_FILES})




install(TARGETS raytracer DESTINATION .)

# Build tests
add_executable(tests
        ../framework/prog5/tests.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Sphere.cpp ../framework/prog5/Box.cpp ../framework/prog5/Shape.cpp ../framework/prog5/Ray.cpp ../framework/prog5/HitPoint.cpp)
target_link_libraries(tests
  ${GLFW_LIBRARIES}
  )



#if it does not work, remove below
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")#remove if it does not work
SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")#remove if it does not work

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")#remove if it does not work
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")#remove if it does not work
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")#remove if it does not work
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")#remove if it does not work


