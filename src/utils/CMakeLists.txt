file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(utils ${source} ${header})

find_package(nlohmann_json 3.2.0 REQUIRED)
set_target_properties(utils PROPERTIES POSITION_INDEPENDENT_CODE True)
set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(utils pontilus_compile_flags nlohmann_json::nlohmann_json)
target_include_directories(utils PRIVATE "${CMAKE_SOURCE_DIR}/src/vendor/json/single_include")

install(FILES ${header} DESTINATION include)