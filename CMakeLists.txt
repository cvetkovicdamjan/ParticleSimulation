cmake_minimum_required(VERSION 3.16.0)
project(particle_simulation)
cmake_policy(SET CMP0072 NEW)


add_executable(particle_simulation src/main.cpp)

add_library(GLAD "thirdparty/glad.c")

add_subdirectory(util)
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
	"${CMAKE_SOURCE_DIR}"
)

target_link_libraries(particle_simulation
                            util
                            glfw
                            GLAD)
