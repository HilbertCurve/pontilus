file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(core ${source} ${header})

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(core PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(core pontilus_compile_flags audio ecs)

install(FILES ${header} DESTINATION include)