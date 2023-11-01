file(GLOB_RECURSE source glfw/*.cpp glad/*.c)
file(GLOB header glfw/*.h stb/*.h glad/*.h)

add_library(vendor ${source} ${header})

set_target_properties(vendor PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    vendor
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(vendor INTERFACE pontilus_compile_flags)

target_include_directories(vendor PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )

install(FILES ${header} DESTINATION include)