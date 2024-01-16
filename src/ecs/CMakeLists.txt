file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(ecs ${source} ${header})

set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ecs PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(ecs pontilus_compile_flags audio library)

install(FILES ${header} DESTINATION include)