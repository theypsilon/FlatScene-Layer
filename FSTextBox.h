#ifndef __TEXTT_BOX_H__
#define __TEXTT_BOX_H__

#include "MessageHandler.h"
#include "FloatPoint.h"
#include "Image.h"
#include <list>
#include <stack>
#include <map>
#include <string>

typedef struct { 
	string cadena;
	TTF_Font* fuente;
	map<Uint16,CImage*> render;
	int size;
}SFont;

typedef struct { 
	Uint16 glyph;
	CFloatPoint* p;
}SChar;

struct SLineText{ 
public:
	list<SChar> letra;
	SFont* fuente;
private:
	SLineText();
	~SLineText();

	friend struct SText;
};

enum TypeColorTBox {
	TCTB_ALL,
	TCTB_TEXT,
	TCTB_BOX
};

struct SEffectText;

class CTextBox {
private:
	SFont* fuente;
	string file;
	int next;

	list<SChar> charInDisplay;
	CPoint upleft;

	SEffectText* fx;

	int timer;
	SDL_Color col;

	CImage* box;
	Uint8 xBox;
	Uint8 yBox;

	int step;
	int maxStep;

	CTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next);

	virtual ~CTextBox();

	friend struct SText;

	virtual int update();

	virtual void deleteBox();

	virtual void createBox();

	virtual int finish();

	friend class CControlOutputText;
	friend class CScreen;
};

#endif