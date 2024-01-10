file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(physics2d ${source} ${header})

set_target_properties(physics2d PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(physics2d PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(physics2d pontilus_compile_flags)

install(FILES ${header} DESTINATION include)