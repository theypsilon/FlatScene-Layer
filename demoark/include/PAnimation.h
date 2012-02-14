#ifndef __PANIMATION_H__
#define __PANIMATION_H__

#include "FSMessageHandler.h"
#include "FSSpriteSet.h"
#include <string>
#include <vector>

class PControlAnimation;
class PImage;

class PAnimation : public FSMessageHandler {
private:
    PControlAnimation* control;
    PImage* img;
    Uint32 _retardo;
public:
    PAnimation(int num,int* sequence, int sptset);
    ~PAnimation();
    void paso_a_paso();
    FSSprite* paso_a_paso_spt();
    Uint32 retardo();
};

class PControlAnimation {
private:
    int* sequence;
    int num;
    int paso;
public:
    PControlAnimation(int num,int* sequence);
    ~PControlAnimation();

    int cuadro();
    bool primero();
    int avanzar();
    void reiniciar();
};

class PImage {
private:
    int sptSet;
public:
    PImage(int sptSet);
    ~PImage();
    void dibujar(int spt);
    FSSprite* dame(int spt);
};

#endif