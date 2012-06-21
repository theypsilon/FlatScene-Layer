include(HPHPSetup)

set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

file ( GLOB_RECURSE SRCS src/*.cpp )
file ( GLOB HDRS include/*.h )

include_directories( include )

add_library ( flatscene ${SRCS} ${HDRS} )

hphp_link( flatscene )
