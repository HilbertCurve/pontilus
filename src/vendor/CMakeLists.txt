file(GLOB source glfw/*.cpp glad/*.c)
file(GLOB header glfw/*.h stb/*.h glad/*.h)

add_library(vendor ${source} ${header})
find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(vendor nlohmann_json)
target_link_libraries(foo PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(vendor PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    vendor
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(vendor PRIVATE
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor/json/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )

target_link_libraries(vendor INTERFACE pontilus_compile_flags)

install(FILES ${header} DESTINATION include)