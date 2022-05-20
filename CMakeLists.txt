cmake_minimum_required(VERSION 3.16)
set(CMAKE_SUPPRESS_REGENERATION true)

set(PROJECT_NAME GLCompute)

set (CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME} CXX)

include_directories(.)
include_directories(ImApp/ImApp/submodules/src)
include_directories(ImApp/ImApp/submodules/src/imgui)
include_directories(ImApp/ImApp/submodules/src/glfw/include)
include_directories(ImApp/ImApp/submodules/src/glm)
include_directories(ImApp/ImApp/submodules/glad/include)
include_directories(ImApp/ImApp/submodules/src/stb)
include_directories(ImApp/ImApp/src/)

set(EXTERNAL TRUE)

add_subdirectory(imapp ${CMAKE_CURRENT_BINARY_DIR}/ImApp/ImApp/)
add_subdirectory(glcompute ${CMAKE_CURRENT_BINARY_DIR}/GLCompute)

target_link_libraries(GLCompute PRIVATE ImApp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLCompute)