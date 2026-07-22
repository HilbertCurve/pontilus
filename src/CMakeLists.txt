
add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(renderer)
# add_subdirectory(gui)
add_subdirectory(ptlib)
add_subdirectory(math)
add_subdirectory(model)
# add_subdirectory(physics2d)
# add_subdirectory(postprocessing)
add_subdirectory(utils)

add_library(pontilus_src INTERFACE)
target_link_libraries(pontilus_src INTERFACE audio core ecs renderer ptlib math model utils)

target_include_directories(pontilus_src INTERFACE
                           "${CMAKE_SOURCE_DIR}/include"
                           "${CMAKE_SOURCE_DIR}/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )



