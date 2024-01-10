file(GLOB_RECURSE source *.cpp)
FOREACH(item ${source})
  IF(${item} MATCHES "vendor/stb/*")
    LIST(REMOVE_ITEM source ${item})
  ENDIF(${item} MATCHES "vendor/stb/*")
  IF(${item} MATCHES "vendor/json/*")
    LIST(REMOVE_ITEM source ${item})
  ENDIF(${item} MATCHES "vendor/json/*")
ENDFOREACH(item)

file(
    GLOB header
    glfw/include/*.h
    stb/*.h
)

list(APPEND source "${CMAKE_SOURCE_DIR}/src/vendor/glad/gl.c")
list(APPEND header "${CMAKE_SOURCE_DIR}/src/vendor/glad/gl.h")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/vendor/glfw")

find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(vendor ${source} ${header})
target_link_libraries(vendor INTERFACE nlohmann_json::nlohmann_json glfw)

set_target_properties(vendor PROPERTIES POSITION_INDEPENDENT_CODE True)
set_target_properties(vendor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(vendor INTERFACE pontilus_compile_flags)
target_include_directories(vendor PRIVATE "${CMAKE_SOURCE_DIR}/src/vendor")

install(FILES ${header} DESTINATION include)