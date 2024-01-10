# geometry project
file(GLOB geometry_source geometry/*.cpp)
file(GLOB geometry_header geometry/*.h)

add_library(math_geometry ${geometry_source} ${geometry_header})
set_target_properties(math_geometry PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(math_geometry PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(math_geometry pontilus_compile_flags)

# number project
file(GLOB number_source number/*.cpp)
file(GLOB number_header number/*.h)

add_library(math_number ${number_source} ${number_header})
set_target_properties(math_number PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(math_number PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(math_number pontilus_compile_flags)

# set project
file(GLOB set_source set/*.cpp)
file(GLOB set_header set/*.h)

add_library(math_set ${set_source} ${set_header})
set_target_properties(math_set PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(math_set PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(math_set pontilus_compile_flags)

# zipped together
add_library(math Math.h)
target_link_libraries(math math_geometry math_number math_set)
set_target_properties(math PROPERTIES POSITION_INDEPENDENT_CODE True)
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(math pontilus_compile_flags)

install(FILES ${header} DESTINATION include)
