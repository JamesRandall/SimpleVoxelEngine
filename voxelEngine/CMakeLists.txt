cmake_minimum_required(VERSION 3.10)

project(
  SimpleVoxelEngine
  VERSION 1.0
  LANGUAGES CXX)


add_library(voxelengine src/AABBTree.cpp
src/AABBTree.h
src/BehaviourFactory.cpp
src/Camera.cpp
src/Camera.h
src/Chunk.cpp
src/ChunkManager.cpp
src/ChunkManager.h
src/Controls.cpp
src/Controls.hpp
src/FrameCounter.h
src/ISprite.cpp
src/IVoxel.cpp
src/MouseAndKeyboardCameraControllerInput.cpp
src/NotImplementedException.h
src/NullCameraController.h
src/ShaderManager.cpp
src/ShaderManager.h
src/SpriteCollisionSet.h
src/SpriteManager.cpp
src/SpriteManager.h
src/SpriteVoxel.h
src/targetver.h
src/UniformChunk.cpp
src/VoxelContainerGeometry.cpp
src/VoxelContainerGeometry.h
src/VoxelEngine.cpp
src/VoxelGeometry.cpp
src/VoxelGeometry.h
src/VoxelRenderer.cpp
src/VoxelRenderer.h
src/VoxelSprite.cpp
src/VoxelSprite.h
src/behaviours/VoxelatedConstructionSpriteBehaviour.cpp
src/behaviours/VoxelatedConstructionSpriteBehaviour.h
src/shaders/grass.fragmentshader
src/shaders/simpleLightColor.fragmentshader
src/shaders/simpleLightTransform.vertexshader
src/shaders/simpleTransform.vertexshader
src/shaders/simple.vertexshader
include/AABB.h
include/BehaviourFactory.h
include/Chunk.h
include/color.h
include/GeometryUpdateRequest.h
include/IAABB.h
include/ICameraControllerInput.h
include/ICamera.h
include/IChunkFactory.h
include/IChunk.h
include/IChunkManager.h
include/ILightSource.h
include/ISpriteBehaviour.h
include/ISpriteCollisionSet.h
include/ISprite.h
include/ISpriteManager.h
include/ISpriteVoxel.h
include/IVoxelContainer.h
include/IVoxel.h
include/MouseAndKeyboardCameraControllerInput.h
include/SimpleLight.h
include/SpriteCollision.h
include/UniformChunk.h
include/VoxelEngineException.h
include/VoxelEngine.h
include/worldSize.h
 )

target_include_directories(voxelengine PRIVATE include/ ../depends/include/ src/)


