file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(graphics ${source} ${header})

set_target_properties(graphics PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(graphics PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(graphics pontilus_compile_flags)

install(FILES ${header} DESTINATION include)