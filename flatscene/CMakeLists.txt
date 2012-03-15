project ( flatscene )

cmake_minimum_required ( VERSION 2.6 )
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

find_package ( OpenGL )

find_package ( SDL REQUIRED )
find_package ( SDL_image REQUIRED )
find_package ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL ERROR "SDL not found!" )	
endif ( NOT SDL_FOUND )

file ( GLOB_RECURSE SRCS src/*.cpp )
file ( GLOB HDRS include/*.h )

include_directories( ${SDL_INCLUDE_DIR} )
include_directories( ${OPENGL_INCLUDE_DIR} )
include_directories( include )

add_library ( flatscene ${SRCS} ${HDRS} )

target_link_libraries ( flatscene
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLTTF_LIBRARY}
	SDLmain
	${OPENGL_LIBRARY}
)
