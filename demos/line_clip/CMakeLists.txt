
add_executable(CPURENDER_DEMO_LINE_CLIP "line_clip_demo.cpp")
target_link_libraries(CPURENDER_DEMO_LINE_CLIP libolcPGE_BIND)
include_directories(../../include)
include_directories(../../backend_binds/olcPGE/)
