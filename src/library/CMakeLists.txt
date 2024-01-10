file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(library ${source} ${header})

set_target_properties(library PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(library PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(library pontilus_compile_flags)

install(FILES ${header} DESTINATION include)