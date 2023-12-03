file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(platformer_ext ${source} ${header})

set_target_properties(platformer_ext PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    platformer_ext
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(platformer_ext PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )
