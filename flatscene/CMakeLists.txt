PROJECT ( flatscene )

CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

FIND_PACKAGE ( OpenGL )

FIND_PACKAGE ( SDL REQUIRED )
FIND_PACKAGE ( SDL_image REQUIRED )
FIND_PACKAGE ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL ERROR "SDL not found!" )	
endif ( NOT SDL_FOUND )

file ( GLOB SRCS src/*.cpp src/tinyxml/*.cpp )
file ( GLOB HDRS include/*.h include/tinyxml/*.h )

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
