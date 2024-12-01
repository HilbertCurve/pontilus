file(GLOB source *.cpp)
file(GLOB header *.h)

add_executable(rpg "${CMAKE_SOURCE_DIR}/examples/rpg_game/rpg.cpp"
        Player.cpp
        Player.h
        Enemy.cpp
        Enemy.h)
target_link_libraries(rpg PUBLIC pontilus)

set_target_properties(rpg PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    rpg
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(rpg PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )
