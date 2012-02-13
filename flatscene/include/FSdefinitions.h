#ifndef ___DEFINITIONS____
#define ___DEFINITIONS____


//OPENGL 1.2


#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D


// LIBRARY
// instantiate the singletons before the main begins and create global
// references for his use
#define GLOBAL_SINGLETON_REFERENCES

#define TEXT_BASE_SCALE 2
#define BITMODE 32

#define EXITO 1
#define FRACASO -1

#define INSTANT_PRINT_ERROR
//#define IN_FILE_ERROR

// Activa el módulo que genera el texto gráfico.
#define TEXT_OPERATIVE

#define MAINRENDERLOOP

// Canvas en OpenGL : ONLY_TEXTURE , WITH_SDL_SURFACE, ONLY_SDL_SURFACE
#define ONLY_TEXTURE 0x01
#define WITH_SDL_SURFACE 0x02
#define ONLY_SDL_SURFACE 0x04

#define ALL_TEXT -1

#define LOG_SISTEMA
#define LOG_SPRITESET

// Activa código que facilita la depuración del programa en ejecución : Desactiva la consideración de nuevos eventos SDL (Por ahora todos los eventos de este tipo son de I/O)
//#define DEBUGTEST
// En VS08 activa un detector que avisa si encuentra memory leaks, si está correctamente configurado.
//#define VISUAL_LEAKS
// Ticks máximos que está el sistema en estado de debug, si llega a entrar.
//#define DEBUGTESTTICKS 100

#define DEBUGLOGS

#ifndef DEBUGLOGS 
	#ifdef LOG_SISTEMA 
		#undef LOG_SISTEMA 
	#endif 
	#ifdef LOG_SPRITESET
		#undef LOG_SPRITESET
	#endif 
#endif

#ifdef LOG_SPRITESET_INFO
	#define LOG_SPRITESET
#endif


// TESTS

//#define RESERVA_MEMORIA_SEGURA
#ifdef RESERVA_MEMORIA_SEGURA
	#define alloc(A,B) new A[B]
	#define freemem(A) delete[](A)
#else
	#define alloc(A,B) (A*)malloc((B)*sizeof(A))
	#define freemem(A) free(A)
#endif

#define TAM_TILE 32
#define RESOLUCION_X 320
#define RESOLUCION_Y 240
#define COMPDUREZAS 6
// Cambia el modo de lectura de eventos del teclado. Ahorro de memoria VS Rapidez ( !EVENTOS_RAPIDO vs EVENTOS_RAPIDO )

// TEST de Zoom en cámara.
//#define __TESTZOOM__

#define EVENTOS_RAPIDO

// Si no está activado, puede verse un pequeño efecto columna si hay muchisimos elementos en pantalla. Probado con la demo de 10000 NPCs.
#define ACTORES_POR_ORDEN_EXACTO

// Rango de teclas usables del teclado.
#define NUM_SDLKEY 323

// Número de botones por Jugador.
#define NUM_PJKEY 16

// Habilita printfs con salto de línea para debug: LOG_MAPAS | LOG_JUGADORES | LOG_ACCIONES | LOG_SISTEMA | LOG_SPRITESET
#define LOG_MAPAS
#define LOG_JUGADORES
//#define LOG_ACCIONES
// Habilita printfs con retorno de carro para debug: MENSAJES_COLISIONCOLOR | MENSAJES_ACCIONES

//#define MENSAJES_ACCIONES
#define MENSAJES_FPS
//#define MENSAJES_COORDENADAS
//#define MENSAJES_MSGIDS

#define DEBUGTESTLOGS

#ifndef DEBUGTESTLOGS 
	#ifdef LOG_MAPAS 
		#undef LOG_MAPAS 
	#endif 
	#ifdef LOG_JUGADORES 
		#undef LOG_JUGADORES 
	#endif 
	#ifdef LOG_ACCIONES 
		#undef LOG_ACCIONES 
	#endif 
#endif

// Nombres de Teclas.

#define PKEY_UP 0
#define PKEY_DOWN 1
#define PKEY_LEFT 2
#define PKEY_RIGHT 3
#define PKEY_B1 4
#define PKEY_B2 5
#define PKEY_B3 6
#define PKEY_B4 7
#define PKEY_B5 8
#define PKEY_B6 9
#define PKEY_B7 10
#define PKEY_B8 11

#endif
