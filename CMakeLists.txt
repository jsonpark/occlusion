cmake_minimum_required(VERSION 3.1.0)
project(occlusion)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++ thread
#find_package(Threads)

# fPIC
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# eigen
#find_package(Eigen3 REQUIRED)

# assimp
#find_package(assimp REQUIRED)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/playpen/jaesungp/lib/glfw-3.2.1 build)
include_directories(/playpen/jaesungp/lib/glfw-3.2.1/include)

# Excutable to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(
  src
  lib
  #${EIGEN3_INCLUDE_DIRS}
  #${ASSIMP_INCLUDE_DIRS}
  #${OPENGL_INCLUDE_DIR}
)

set(OCCLUSION_SOURCE_FILES
  # GLAD
  lib/glad/glad.h
  lib/glad/glad.c
  lib/KHR/khrplatform.h

  # tinyxml2
  #lib/tinyxml2/tinyxml2.h
  #lib/tinyxml2/tinyxml2.cpp

  src/occlusion/dataset.h
  src/occlusion/dataset.cc
  src/occlusion/dataset_utkinect.h
  src/occlusion/dataset_utkinect.cc
  src/occlusion/engine.h
  src/occlusion/engine.cc
  src/occlusion/program.h
  src/occlusion/program.cc
  src/occlusion/robot_model.h
  src/occlusion/robot_model.cc
  src/occlusion/robot_model_loader.h
  src/occlusion/robot_model_loader.cc
  src/occlusion/shader.h
  src/occlusion/shader.cc
  src/occlusion/texture.h
  src/occlusion/texture.cc

  src/occlusion/main.cc
)

add_executable(occlusion
  ${OCCLUSION_SOURCE_FILES}
)

target_link_libraries(occlusion
  #Threads::Threads
  #${ASSIMP_LIBRARIES}
  glfw
  stdc++fs
)
