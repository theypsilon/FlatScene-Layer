#include "PAnimation.h"
#include "FSImages.h"
#include "FSLibrary.h"

PAnimation::PAnimation(int num ,int* sequence, int sptset ) : FSMessageHandler(NULL), control(new PControlAnimation(num,sequence)), img(new PImage(sptset)), _retardo(16) {
}

PAnimation::~PAnimation() {
	delete control;
	delete img;
}

Uint32 PAnimation::retardo() {
	return _retardo;
}

void PAnimation::paso_a_paso() {
	img->dibujar(control->avanzar());
}

FSSprite* PAnimation::paso_a_paso_spt() {
	return img->dame(control->avanzar());
}


PControlAnimation::PControlAnimation(int num,int* sequence) : num(num), sequence(sequence), paso(0) {
}

PControlAnimation::~PControlAnimation() {
	freemem(sequence);
}

bool PControlAnimation::primero() {
	return paso==0;
}

int PControlAnimation::avanzar() {
	paso++;
	if (paso>=num)
		paso=0;
	return cuadro();
}

int PControlAnimation::cuadro() {
	return sequence[paso];
}

void PControlAnimation::reiniciar() {
	paso=0;
}

PImage::PImage(int sptSet) : sptSet(sptSet) {

}
PImage::~PImage() {

}
void PImage::dibujar(int spt) {
	FSPoint pt(100,100);
	Img.get(sptSet)->get(spt)->put(pt);
}

FSSprite* PImage::dame(int spt) {
	return Img.get(sptSet)->get(spt);
}
