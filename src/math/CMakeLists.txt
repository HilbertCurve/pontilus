# set project
file(GLOB set_source set/*.cpp)

# geometry project
file(GLOB geometry_source geometry/*.cpp)

# number project
file(GLOB number_source number/*.cpp)

# zipped together
add_library(math ${set_source} ${geometry_source} ${number_source})
set_target_properties(math PROPERTIES POSITION_INDEPENDENT_CODE True)
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(math pontilus_compile_flags)

install(FILES ${header} DESTINATION include)
