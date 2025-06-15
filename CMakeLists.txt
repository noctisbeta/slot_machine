cmake_minimum_required(VERSION 3.10.0)
project(slot_machine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(slot_machine src/main.cpp)

target_link_libraries(slot_machine PRIVATE glfw GLEW::GLEW glm::glm)

