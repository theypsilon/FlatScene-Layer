#include "PAnimation.h"
#include "ControlImages.h"
#include "Library.h"

PAnimation::PAnimation(int num ,int* sequence, int sptset ) : CMessageHandler(NULL), control(new PControlAnimation(num,sequence)), img(new PImage(sptset)), _retardo(16) {
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

CSprite* PAnimation::paso_a_paso_spt() {
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
	CPoint pt(100,100);
	CImg.get(sptSet)->get(spt)->put(pt);
}

CSprite* PImage::dame(int spt) {
	return CImg.get(sptSet)->get(spt);
}
