PROJECT ( flatscene )

CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )

FIND_PACKAGE ( Boost COMPONENTS function )

FIND_PACKAGE ( OpenGL )

FIND_PACKAGE ( SDL REQUIRED )
FIND_PACKAGE ( SDL_image REQUIRED )
FIND_PACKAGE ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL ERROR "SDL not found!" )	
endif ( NOT SDL_FOUND )

file ( GLOB SRCS flatscene/*.cpp )
file ( GLOB HDRS flatscene/*.cpp )

add_library ( flatscene ${SRCS} ${HDRS} )

include_directories( ${SDL_INCLUDE_DIR} )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${OPENGL_INCLUDE_DIR} )

target_link_libraries ( flatscene
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLTTF_LIBRARY}
	SDLmain
	${Boost_LIBRARIES}
	${OPENGL_LIBRARY}
)

file ( GLOB NSRCS demoark/*.cpp )
file ( GLOB NHDRS demoark/*.h )

add_executable ( testdemoark ${NSRCS} ${NHDRS} )

include_directories ( ./flatscene/ )

target_link_libraries ( testdemoark 
	flatscene
)

