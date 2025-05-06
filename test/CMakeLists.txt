cmake_minimum_required(VERSION 2.8.1)
project(TestCPURenderProj)

add_subdirectory("./unittest-cpp-2.0.0")

file(GLOB CPU_REN_TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp tests/*.h)


add_executable(TestCPURender ${CPU_REN_TEST_SRCS})
include_directories(./unittest-cpp-2.0.0)
include_directories(./unittest-cpp-2.0.0)
target_link_libraries(TestCPURender UnitTest++)
target_link_libraries(TestCPURender libcpurender)
