file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(model ${source} ${header})

set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    model
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(model PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )

install(FILES ${header} DESTINATION include)