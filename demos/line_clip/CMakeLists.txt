

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
add_executable(CPURENDER_DEMO_LINE_CLIP "line_clip_demo.cpp")
target_link_libraries(CPURENDER_DEMO_LINE_CLIP libolcPGE_BIND)
target_link_libraries(CPURENDER_DEMO_LINE_CLIP libcpurender)
include_directories(../../include)
include_directories(../../backend_binds/olcPGE/)
