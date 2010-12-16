#ifndef __COLOR_H__
#define __COLOR_H__

#include "SDL/SDL.h"

class CColor  
{
private:
	SDL_Color m_Color;
public:

	CColor(Uint8 r=0,Uint8 g=0,Uint8 b=0);
	CColor(const CColor& Color);
	virtual ~CColor();

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

	CColor& operator=(CColor& Color);
	CColor& operator+=(CColor& Color);
	CColor& operator-=(CColor& Color);
	CColor& operator*=(CColor& Color);
	CColor& operator*=(int Multiplier);
	CColor& operator/=(int Divisor);
	CColor& operator|=(CColor& Color);
	CColor& operator&=(CColor& Color);
	CColor& operator^=(CColor& Color);

	static CColor Red(Uint8 shade=255);
	static CColor Green(Uint8 shade=255);
	static CColor Blue(Uint8 shade=255);

	static CColor Yellow(Uint8 shade=255);
	static CColor Cyan(Uint8 shade=255);
	static CColor Magenta(Uint8 shade=255);

	static CColor DarkRed(Uint8 shade=128);
	static CColor DarkGreen(Uint8 shade=128);
	static CColor DarkBlue(Uint8 shade=128);
	static CColor DarkYellow(Uint8 shade=128);
	static CColor DarkCyan(Uint8 shade=128);
	static CColor DarkMagenta(Uint8 shade=128);

	static CColor LightRed(Uint8 gray=128,Uint8 shade=255);
	static CColor LightGreen(Uint8 gray=128,Uint8 shade=255);
	static CColor LightBlue(Uint8 gray=128,Uint8 shade=255);
	static CColor LightYellow(Uint8 gray=128,Uint8 shade=255);
	static CColor LightCyan(Uint8 gray=128,Uint8 shade=255);
	static CColor LightMagenta(Uint8 gray=128,Uint8 shade=255);

	static CColor White(Uint8 shade=255);
	static CColor LightGray(Uint8 shade=192);
	static CColor DarkGray(Uint8 shade=128);
	static CColor Black(Uint8 shade=0);
};

CColor operator+(CColor& Color1,CColor& Color2);
CColor operator-(CColor& Color1,CColor& Color2);
CColor operator*(CColor& Color1,CColor& Color2);
CColor operator*(CColor& Color,int Multiplier);
CColor operator/(CColor& Color,int Divisor);

CColor operator|(CColor& Color1,CColor& Color2);
CColor operator&(CColor& Color1,CColor& Color2);
CColor operator^(CColor& Color1,CColor& Color2);
CColor operator~(CColor& Color);

bool operator==(CColor& Color1,CColor& Color2);
bool operator!=(CColor& Color1,CColor& Color2);

#endif //#ifndef __COLOR_H__
