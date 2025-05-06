
add_executable(CPURENDER_APP ./debug.cpp)

target_link_libraries(CPURENDER_APP libcpurender)
include_directories(../include)
include_directories(../src)