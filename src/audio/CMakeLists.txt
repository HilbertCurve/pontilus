file(GLOB source *.cpp)
file(GLOB header *.h)

add_library(audio ${source} ${header})

set_target_properties(audio PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(audio PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(audio pontilus_compile_flags openal)

install(FILES ${header} DESTINATION include)
