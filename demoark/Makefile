FILES=	Action.cpp \
	ActionComboDemo0.cpp \
	ActionComboDemo1.cpp \
	ActionFight.cpp \
	ActionMove.cpp \
	ActionQuiet0.cpp \
	ActionQuietAuto0.cpp \
	ActionSimpleDemo0.cpp \
	ActionWalk0.cpp \
	ActionWalkAuto0.cpp \
	ActionXPunto.cpp \
	Actor.cpp \
	ActorScrollMap.cpp \
	ActorMouse.cpp \
	Animation.cpp \
	Library.cpp \
	Camera.cpp \
	CameraOS.cpp \
	Color.cpp \
	ControlImages.cpp \
	ControlMultiverse.cpp \
	Writer.cpp \
	Coordinate.cpp \
	Enemy.cpp \
	EnemyNPC.cpp \
	EnemyPunto.cpp \
	Engine.cpp \
	FloatPoint.cpp \
	FreezeGameInterface.cpp \
	GarbageCollector.cpp \
	Image.cpp \
	LayerBackground.cpp \
	main.cpp \
	Map.cpp \
	MenuAGameInterface.cpp \
	MessageHandler.cpp \
	Node.cpp \
	OrderGraph.cpp \
	PAnimation.cpp \
	Player.cpp \
	Point.cpp \
	Rectangle.cpp \
	RutinaColision.cpp \
	Screen.cpp \
	ScrollCamera.cpp \
	SDL_rotozoom.cpp \
	Sprite.cpp \
	SpriteSet.cpp \
	TestA0GameInterface.cpp \
	TestA1GameInterface.cpp \
	TestA2GameInterface.cpp \
	TestA3GameInterface.cpp \
	TestA4GameInterface.cpp \
	TestA5GameInterface.cpp \
	TestAGameInterface.cpp \
	TestBGameInterface.cpp \
	TestOSDesktopInterface.cpp \
	TextBox.cpp \
	Time.cpp \
	tinystr.cpp \
	tinyxml.cpp \
	tinyxmlerror.cpp \
	tinyxmlparser.cpp \
	Universe.cpp

OFILES= Action.o \
	ActionComboDemo0.o \
	ActionComboDemo1.o \
	ActionFight.o \
	ActionMove.o \
	ActionQuiet0.o \
	ActionQuietAuto0.o \
	ActionSimpleDemo0.o \
	ActionWalk0.o \
	ActionWalkAuto0.o \
	ActionXPunto.o \
	Actor.o \
	ActorScrollMap.o \
	ActorMouse.o \
	Animation.o \
	Library.o \
	Camera.o \
	CameraOS.o \
	Color.o \
	ControlImages.o \
	ControlMultiverse.o \
	Writer.o \
	Coordinate.o \
	Enemy.o \
	EnemyNPC.o \
	EnemyPunto.o \
	Engine.o \
	FloatPoint.o \
	FreezeGameInterface.o \
	GarbageCollector.o \
	Image.o \
	LayerBackground.o \
	main.o \
	Map.o \
	MenuAGameInterface.o \
	MessageHandler.o \
	Node.o \
	OrderGraph.o \
	PAnimation.o \
	Player.o \
	Point.o \
	Rectangle.o \
	RutinaColision.o \
	Screen.o \
	ScrollCamera.o \
	SDL_rotozoom.o \
	Sprite.o \
	SpriteSet.o \
	TestA0GameInterface.o \
	TestA1GameInterface.o \
	TestA2GameInterface.o \
	TestA3GameInterface.o \
	TestA4GameInterface.o \
	TestA5GameInterface.o \
	TestAGameInterface.o \
	TestBGameInterface.o \
	TestOSDesktopInterface.o \
	TextBox.o \
	Time.o \
	tinystr.o \
	tinyxml.o \
	tinyxmlerror.o \
	tinyxmlparser.o \
	Universe.o

LIBS= -lGL -lGLU `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf

NOMBRE= release

all: $(OFILES)
	g++ -g -o $(NOMBRE) $(OFILES) $(LIBS)

clean:
	rm -f *.o *~

debug: $(FILES)
	g++ $(FILES) -lncurses -ggdb -o $(NOMBRE) $(LIBS)
