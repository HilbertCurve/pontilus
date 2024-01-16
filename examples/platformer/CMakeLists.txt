set(platformerSources main.cpp player.cpp tilemap.cpp editor.cpp)
set(platformerHeaders player.h tilemap.h editor.h)

add_executable(platformer ${platformerSources} ${platformerHeaders})
target_link_libraries(platformer PRIVATE pontilus)
