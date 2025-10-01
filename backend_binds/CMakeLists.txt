
add_library(libolcPGE_BIND STATIC ./olcPGE/olcPGEBind.cpp)
target_include_directories(libolcPGE_BIND PRIVATE "./olcPGE/")
target_include_directories(libolcPGE_BIND PRIVATE "../include/")
target_include_directories(libolcPGE_BIND PRIVATE "./")
target_link_libraries(libolcPGE_BIND libcpurender)

