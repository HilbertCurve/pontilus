cmake_minimum_required(VERSION 3.10)

project(pontilus VERSION 0.1)

add_library(pontilus_compile_flags INTERFACE)
target_compile_features(pontilus_compile_flags INTERFACE cxx_std_11)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,GCC,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(pontilus_compile_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wshadow;-Wformat=2;-Wunused;-D_GLIBCXX_USE_CXX11_ABI=0 >>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
target_include_directories(pontilus_compile_flags
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/json/single_include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/glfw/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor>
)

# set(ALSOFT_EXAMPLES=OFF)
# add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/openal-soft")

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glm CONFIG REQUIRED)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
add_subdirectory(src)
add_subdirectory(vendor)

add_library(pontilus src)
set_target_properties(pontilus PROPERTIES LINKER_LANGUAGE CXX POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(pontilus PUBLIC pontilus_compile_flags pontilus_src OpenGL openal glm::glm)

target_include_directories(pontilus INTERFACE
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/vendor"
                           )
# todo: pull dependencies

