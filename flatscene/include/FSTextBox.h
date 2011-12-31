#ifndef __TEXTT_BOX_H__
#define __TEXTT_BOX_H__

#include "FSMessageHandler.h"
#include "FSPoint.h"
#include "FSImage.h"
#include <list>
#include <stack>
#include <map>
#include <string>

enum TypeColorTBox {
	TCTB_ALL,
	TCTB_TEXT,
	TCTB_BOX
};
struct SFont { 
	string cadena;
	TTF_Font* fuente;
	map<Uint16,FSImage*> render;
	int size;
};

struct SChar { 
	Uint16 glyph;
	FSFloatPoint* p;
};

struct SLineText{ 
public:
	list<SChar> letra;
	SFont* fuente;
private:
	SLineText();
	~SLineText();
	friend struct SText;
};


struct SEffectText;

class FSTextBox {
private:
	SFont* fuente;
	string file;
	int next;

	list<SChar> charInDisplay;
	FSPoint upleft;

	SEffectText* fx;

	int timer;
	SDL_Color col;

	FSImage* box;
	Uint8 xBox;
	Uint8 yBox;

	int step;
	int maxStep;

	FSTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next);

	virtual ~FSTextBox();

	friend struct SText;

	virtual int update();

	virtual void deleteBox();

	virtual void createBox();

	virtual int finish();

	friend class FSControlOutputText;
	friend class FSScreen;
};

#endif