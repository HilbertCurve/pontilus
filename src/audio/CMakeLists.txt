file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(audio ${source} ${header})

set_target_properties(audio PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    audio
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(audio PROPERTIES POSITION_INDEPENDENT_CODE True)

target_include_directories(audio PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )

install(FILES ${header} DESTINATION include)
