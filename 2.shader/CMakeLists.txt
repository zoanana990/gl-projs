cmake_minimum_required(VERSION 3.24)
project(2_shader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(/opt/homebrew/Cellar/glew/2.2.0_1/include)
include_directories(/opt/homebrew/Cellar/glfw/3.3.4/include)
link_directories(/opt/homebrew/Cellar/glew/2.2.0_1/lib)

# notice your glfw version
link_directories(/opt/homebrew/Cellar/glfw/3.3.8/lib)

add_executable(2_shader main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(2_shader PRIVATE OpenGL::GL)
target_link_libraries(2_shader PRIVATE glfw)
target_link_libraries(2_shader PRIVATE GLEW)
