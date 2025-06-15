cmake_minimum_required(VERSION 3.10.0)
project(slot_machine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(slot_machine src/main.cpp src/Shader.cpp src/ShaderProgram.cpp)

target_link_libraries(slot_machine PRIVATE glfw GLEW::GLEW glm::glm)

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(DESTINATION_DIR $<TARGET_FILE_DIR:slot_machine>/assets)
if(EXISTS ${ASSETS_DIR})
    add_custom_command(
        TARGET slot_machine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${DESTINATION_DIR}
        COMMENT "Copying assets to build directory: ${ASSETS_DIR} -> ${DESTINATION_DIR}"
    )
else()
    message(WARNING "Assets directory not found at ${ASSETS_DIR}. Make sure it exists at the root of your project.")
endif()

