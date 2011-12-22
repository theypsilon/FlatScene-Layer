#ifndef __COLOR_H__
#define __COLOR_H__

#include "SDL.h"

class FSColor  
{
private:
	SDL_Color m_Color;
public:

	FSColor(Uint8 r=0,Uint8 g=0,Uint8 b=0);
	FSColor(const FSColor& Color);
	virtual ~FSColor();

	Uint8 getR() const;
	Uint8 getG() const;
	Uint8 getB() const;

	void setR(Uint8 r);
	void setG(Uint8 g);
	void setB(Uint8 b);

	Uint8& R();
	Uint8& G();
	Uint8& B();

	operator SDL_Color();
	operator SDL_Color*();

	FSColor& operator=(FSColor& Color);
	FSColor& operator+=(FSColor& Color);
	FSColor& operator-=(FSColor& Color);
	FSColor& operator*=(FSColor& Color);
	FSColor& operator*=(int Multiplier);
	FSColor& operator/=(int Divisor);
	FSColor& operator|=(FSColor& Color);
	FSColor& operator&=(FSColor& Color);
	FSColor& operator^=(FSColor& Color);

	static FSColor Red(Uint8 shade=255);
	static FSColor Green(Uint8 shade=255);
	static FSColor Blue(Uint8 shade=255);

	static FSColor Yellow(Uint8 shade=255);
	static FSColor Cyan(Uint8 shade=255);
	static FSColor Magenta(Uint8 shade=255);

	static FSColor DarkRed(Uint8 shade=128);
	static FSColor DarkGreen(Uint8 shade=128);
	static FSColor DarkBlue(Uint8 shade=128);
	static FSColor DarkYellow(Uint8 shade=128);
	static FSColor DarkCyan(Uint8 shade=128);
	static FSColor DarkMagenta(Uint8 shade=128);

	static FSColor LightRed(Uint8 gray=128,Uint8 shade=255);
	static FSColor LightGreen(Uint8 gray=128,Uint8 shade=255);
	static FSColor LightBlue(Uint8 gray=128,Uint8 shade=255);
	static FSColor LightYellow(Uint8 gray=128,Uint8 shade=255);
	static FSColor LightCyan(Uint8 gray=128,Uint8 shade=255);
	static FSColor LightMagenta(Uint8 gray=128,Uint8 shade=255);

	static FSColor White(Uint8 shade=255);
	static FSColor LightGray(Uint8 shade=192);
	static FSColor DarkGray(Uint8 shade=128);
	static FSColor Black(Uint8 shade=0);
};

FSColor operator+(FSColor& Color1,FSColor& Color2);
FSColor operator-(FSColor& Color1,FSColor& Color2);
FSColor operator*(FSColor& Color1,FSColor& Color2);
FSColor operator*(FSColor& Color,int Multiplier);
FSColor operator/(FSColor& Color,int Divisor);

FSColor operator|(FSColor& Color1,FSColor& Color2);
FSColor operator&(FSColor& Color1,FSColor& Color2);
FSColor operator^(FSColor& Color1,FSColor& Color2);
FSColor operator~(FSColor& Color);

bool operator==(FSColor& Color1,FSColor& Color2);
bool operator!=(FSColor& Color1,FSColor& Color2);

#endif //#ifndef __COLOR_H__
