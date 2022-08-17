cmake_minimum_required(VERSION 3.10)
SET (PROJECT_NAME perlinLandscape)
project(
  ${PROJECT_NAME}
  VERSION 1.0
  LANGUAGES CXX)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
list(APPEND SRC_FILES)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} voxelengine glad -lX11 -lglfw -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama)

target_include_directories(${PROJECT_NAME} PRIVATE ../../voxelEngine/include/ ../../depends/include/ ../../depends/include/glad/ src/)


