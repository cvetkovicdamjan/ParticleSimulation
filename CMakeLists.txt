cmake_minimum_required(VERSION 3.28.0)
project(particle_simulation LANGUAGES CXX C CUDA)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 17)

add_executable(particle_simulation src/main.cpp src/RigidBody.cpp src/particleSampling.cu)

add_library(GLAD "thirdparty/glad.c")

add_subdirectory(util)
include_directories(
    "/usr/local/cuda-13.0/targets/x86_64-linux/include"
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

target_include_directories(particle_simulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(particle_simulation PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>)
