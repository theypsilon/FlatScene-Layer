#include "FSActor.h"
#include "FSLibrary.h"
#include "FSUniverse.h"

CActor::CActor(const char* creature,CMessageHandler * pmhParent) : 
file(0), graph(0), flags(0), inUniverse(NULL), creature(creature), 
	CMessageHandler(pmhParent) {
	
}
CActor::~CActor() {

}

int CActor::move() {
	return EXITO;
}

CUniverse* CActor::getUniverse() {
	return inUniverse;
}
string CActor::getCreature() {
	return creature;
}

int CActor::setUniverse(CUniverse* m) {
	inUniverse=m;

	return EXITO;
}

int CActor::setSprite(int file,int graph) {
	this->file = file;
	this->graph = graph;

	return EXITO;
}

CSprite* CActor::getSprite() {
	return CImg.get(file)->get(graph);
}

CActor* CActor::clone() {
	CLibrary::Error("CActor no implementa un metodo de clonación por defecto.");
	return NULL;
}