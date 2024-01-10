file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(model ${source} ${header})

set_target_properties(model PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(model PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(model pontilus_compile_flags ecs)

install(FILES ${header} DESTINATION include)