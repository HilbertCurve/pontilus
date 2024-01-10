
add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(graphics)
# add_subdirectory(gui)
add_subdirectory(library)
add_subdirectory(math)
add_subdirectory(model)
add_subdirectory(physics2d)
# add_subdirectory(postprocessing)
add_subdirectory(utils)
add_subdirectory(vendor)

add_library(pontilus_src INTERFACE)
target_link_libraries(pontilus_src INTERFACE audio core ecs graphics library math model utils vendor)

target_include_directories(pontilus_src INTERFACE
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )



