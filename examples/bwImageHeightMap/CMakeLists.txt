cmake_minimum_required(VERSION 3.10)

project(
  bwImageHeightMap
  VERSION 1.0
  LANGUAGES CXX)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
list(APPEND SRC_FILES)
add_executable(bwImageHeightMap ${SRC_FILES})
target_link_libraries(bwImageHeightMap voxelengine glad -lX11 -lglfw -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama)

target_include_directories(bwImageHeightMap PRIVATE ../../voxelEngine/include/ ../../depends/include/ ../../depends/include/glad/ src/)


