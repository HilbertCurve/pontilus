file(GLOB_RECURSE source *.cpp)
FOREACH(item ${source})
  IF(${item} MATCHES "vendor/stb/*")
    LIST(REMOVE_ITEM source ${item})
  ENDIF(${item} MATCHES "vendor/stb/*")
ENDFOREACH(item)


list(APPEND source "${CMAKE_SOURCE_DIR}/src/vendor/glad/gl.c")

file(GLOB_RECURSE header *.h)

list(APPEND header "${CMAKE_SOURCE_DIR}/src/vendor/glad/gl.h")

add_library(pontilus_src ${source} ${header})

set_target_properties(pontilus_src PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(pontilus_src PROPERTIES POSITION_INDEPENDENT_CODE True)

target_include_directories(pontilus_src PUBLIC
                           "${CMAKE_SOURCE_DIR}/src"
                           "${CMAKE_SOURCE_DIR}/src/vendor/glfw/include"
                           "${CMAKE_SOURCE_DIR}/src/vendor"
                           CMAKE_SYSTEM_INCLUDE_PATH
                           )


