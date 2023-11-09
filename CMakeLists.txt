cmake_minimum_required(VERSION 3.10)

project(pontilus VERSION 0.1)

add_library(pontilus_compile_flags INTERFACE)
target_compile_features(pontilus_compile_flags INTERFACE cxx_std_11)



# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(pontilus_compile_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/vendor/glfw")

# set(ALSOFT_EXAMPLES=OFF)
# add_subdirectory("${CMAKE_SOURCE_DIR}/src/vendor/openal-soft")

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
add_subdirectory(src)

add_library(pontilus src)
set_target_properties(pontilus PROPERTIES LINKER_LANGUAGE CXX POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(pontilus PUBLIC pontilus_src pontilus_compile_flags glfw OpenGL openal)

target_include_directories(pontilus PRIVATE
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           )
# todo: pull dependencies

add_executable(mandelbrot "${CMAKE_SOURCE_DIR}/examples/mandelbrot.cpp")
target_link_libraries(mandelbrot PUBLIC pontilus)
add_test(NAME mandelbrot COMMAND mandelbrot)

add_executable(platformer "${CMAKE_SOURCE_DIR}/examples/platformer.cpp")
target_link_libraries(platformer PUBLIC pontilus)
add_test(NAME platformer COMMAND platformer)