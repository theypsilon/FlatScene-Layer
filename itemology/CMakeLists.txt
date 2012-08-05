project ( itemology )

cmake_minimum_required ( VERSION 2.6 )
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")


file ( GLOB_RECURSE NSRCS *.cpp )

add_executable ( item ${NSRCS} )

include_directories ( ../flatscene/include/ )

target_link_libraries ( item 
	flatscene
)

