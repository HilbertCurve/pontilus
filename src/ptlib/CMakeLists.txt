file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(ptlib ${source} ${header})

set_target_properties(ptlib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ptlib PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(ptlib pontilus_compile_flags)

install(FILES ${header} DESTINATION include)
