set(platformerSources main.cpp player.cpp)
set(platformerHeaders player.h)

add_executable(platformer ${platformerSources} ${platformerHeaders})
target_link_libraries(platformer PRIVATE pontilus)
