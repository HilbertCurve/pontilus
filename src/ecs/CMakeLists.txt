set(header
        Entity.h
        Transform.h
        Component.h
        EntityBuilder.h
)
file(GLOB header *.h)

add_library(ecs
        ${header}
        EntityBuilder.cpp
        Component.cpp
        Entity.cpp
        Transform.cpp
)

set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ecs PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(ecs pontilus_compile_flags audio library)

install(FILES ${header} DESTINATION include)