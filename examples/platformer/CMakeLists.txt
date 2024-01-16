set(platformerSources main.cpp player.cpp tilemap.cpp)
set(platformerHeaders player.h tilemap.h)

add_executable(platformer ${platformerSources} ${platformerHeaders})
target_link_libraries(platformer PRIVATE pontilus)
