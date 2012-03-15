PROJECT ( itemology )

CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

FIND_PACKAGE ( OpenGL )

FIND_PACKAGE ( SDL REQUIRED )
FIND_PACKAGE ( SDL_image REQUIRED )
FIND_PACKAGE ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL ERROR "SDL not found!" )	
endif ( NOT SDL_FOUND )


include_directories( ${SDL_INCLUDE_DIR} )
include_directories( ${OPENGL_INCLUDE_DIR} )

file ( GLOB NSRCS *.cpp )
file ( GLOB NHDRS *.h )

add_executable ( item ${NSRCS} )

include_directories ( ../flatscene/include/ )
include_directories ( ./itemology/ )

target_link_libraries ( item 
	flatscene
)

