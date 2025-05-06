cmake_minimum_required(VERSION 3.2)
include(CMakePrintHelpers)

set(CMAKE_CXX_COMPILER "clang++.exe")
set(CMAKE_C_COMPILER "clang.exe")


project(CPURENDER
  VERSION 1.0
  DESCRIPTION "software rendering"    
  LANGUAGES C CXX              
)

if(MSVC14 OR MSVC12)
	# has the support we need
else()
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
	CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	if(COMPILER_SUPPORTS_CXX17)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
	elseif(COMPILER_SUPPORTS_CXX14)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	elseif(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
			message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endif()

set(MAIN_DIR ${CMAKE_SOURCE_DIR})

file(GLOB CPURENDER_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp include/*.h)

add_library(libcpurender STATIC ${CPURENDER_SRCS})

include_directories(./include ./third-party)
set_target_properties(libcpurender PROPERTIES
			CXX_STANDARD 17)

add_subdirectory("./test/")

add_subdirectory("./backend_binds/")

add_subdirectory("./app/")
add_subdirectory("./demos/")