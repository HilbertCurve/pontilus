file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(renderer ${source} ${header})

set_target_properties(renderer PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(renderer PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(renderer pontilus_compile_flags)

install(FILES ${header} DESTINATION include)