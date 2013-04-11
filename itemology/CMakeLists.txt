project ( itemology )

cmake_minimum_required ( VERSION 2.6 )
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -g -std=c++0x")

find_package ( OpenGL )

find_package ( SDL REQUIRED )
find_package ( SDL_image REQUIRED )
find_package ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL ERROR "SDL not found!" )	
endif ( NOT SDL_FOUND )


include_directories( ${SDL_INCLUDE_DIR} )
include_directories( ${OPENGL_INCLUDE_DIR} )

file ( GLOB_RECURSE NSRCS *.cpp )
file ( GLOB NHDRS *.h )

add_executable ( item ${NSRCS} )

include_directories ( ../flatscene/include/ )
include_directories ( ./itemology/ )

target_link_libraries ( item 
	flatscene
)

