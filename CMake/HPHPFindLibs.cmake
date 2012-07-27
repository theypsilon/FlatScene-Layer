#
#   +----------------------------------------------------------------------+
#   | HipHop for PHP                                                       |
#   +----------------------------------------------------------------------+
#   | Copyright (c) 2010 Facebook, Inc. (http://www.facebook.com)          |
#   | Copyright (c) 1997-2010 The PHP Group                                |
#   +----------------------------------------------------------------------+
#   | This source file is subject to version 3.01 of the PHP license,      |
#   | that is bundled with this package in the file LICENSE, and is        |
#   | available through the world-wide-web at the following url:           |
#   | http://www.php.net/license/3_01.txt                                  |
#   | If you did not receive a copy of the PHP license and are unable to   |
#   | obtain it through the world-wide-web, please send a note to          |
#   | license@php.net so we can mail you a copy immediately.               |
#   +----------------------------------------------------------------------+
#

include(CheckFunctionExists)

# OpenGL

find_package ( OpenGL )

if (OPENGL_FOUND)
    message( "-- OpenGL Correctly Found" )
else (OPENGL_FOUND)
    message( FATAL_ERROR "OpenGL environment missing" )
endif (OPENGL_FOUND)

include_directories( ${OPENGL_INCLUDE_DIR} )
link_directories( ${OPENGL_LIBRARY_DIR} )
set( CORE_LIBRARIES ${CORE_LIBRARIES} ${OPENGL_LIBRARY} )


# SDL 

find_package ( SDL REQUIRED )
find_package ( SDL_image REQUIRED )
find_package ( SDL_ttf REQUIRED )

if (SDL_FOUND)
    message( "-- SDL Correctly Found" )
else (SDL_FOUND)
	message ( FATAL_ERROR "SDL not found!" )	
endif (SDL_FOUND)

include_directories( ${SDL_INCLUDE_DIR} )
link_directories( ${SDLIMAGE_LIBRARY_DIR} ${SDLTTF_LIBRARY_DIR} ${SDL_LIBRARY_DIR} )
set( CORE_LIBRARIES ${CORE_LIBRARIES} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} ${SDL_LIBRARY} )

macro(hphp_link target)

	target_link_libraries(${target} ${CORE_LIBRARIES})

endmacro()
