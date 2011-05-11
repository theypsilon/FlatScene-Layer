#include "FSColor.h"
//standard constructor
CColor::CColor(Uint8 r,Uint8 g,Uint8 b)
{
	//set red
	setR(r);
	//get green
	setG(g);
	//set blue
	setB(b);
	//set unused to 0
	m_Color.unused=0;
}

//copy constructor
CColor::CColor(const CColor& Color)
{
	//set red
	setR(Color.getR());
	//set green
	setG(Color.getG());
	//set blue
	setB(Color.getB());
	//set unused to 0
	m_Color.unused=0;
}

//destructor
CColor::~CColor()
{
}

//get rgb
Uint8 CColor::getR() const
{
	//return red
	return(m_Color.r);
}

Uint8 CColor::getG() const
{
	//return green
	return(m_Color.g);
}

Uint8 CColor::getB() const
{
	//return blue
	return(m_Color.b);
}

//set rgb
void CColor::setR(Uint8 r)
{
	//set red
	m_Color.r=r;
}

void CColor::setG(Uint8 g)
{
	//set green
	m_Color.g=g;
}

void CColor::setB(Uint8 b)
{
	//set blue
	m_Color.b=b;
}

//rgb accessors
Uint8& CColor::R()
{
	//return red
	return(m_Color.r);
}

Uint8& CColor::G()
{
	//return green
	return(m_Color.g);
}

Uint8& CColor::B()
{
	//return blue
	return(m_Color.b);
}

//conversion operators
CColor::operator SDL_Color()
{
	//return color
	return(m_Color);
}

CColor::operator SDL_Color*()
{
	//return pointer to color
	return(&m_Color);
}

//assignment operators
CColor& CColor::operator=(CColor& Color)
{
	//set red
	setR(Color.getR());
	//set green
	setG(Color.getG());
	//set blue
	setB(Color.getB());
	//return
	return(*this);
}

CColor& CColor::operator+=(CColor& Color)
{
	//temp var
	int temp;
	//add red
	temp=getR()+Color.getR();
	if(temp>255) temp=255;
	setR(temp);

	//add green
	temp=getG()+Color.getG();
	if(temp>255) temp=255;
	setG(temp);

	//add blue
	temp=getB()+Color.getB();
	if(temp>255) temp=255;
	setB(temp);

	//return
	return(*this);
}

CColor& CColor::operator-=(CColor& Color)
{
	//temp var
	int temp;
	//add red
	temp=getR()-Color.getR();
	if(temp<0) temp=0;
	setR(temp);

	//add green
	temp=getG()-Color.getG();
	if(temp<0) temp=0;
	setG(temp);

	//add blue
	temp=getB()-Color.getB();
	if(temp<0) temp=0;
	setB(temp);

	//return
	return(*this);
}

CColor& CColor::operator*=(CColor& Color)
{
	//temp var
	int temp;
	//add red
	temp=(getR()*Color.getR())/255;
	setR(temp);

	//add green
	temp=(getG()*Color.getG())/255;
	setG(temp);

	//add blue
	temp=(getB()*Color.getB())/255;
	setB(temp);

	//return
	return(*this);
}

CColor& CColor::operator*=(int Multiplier)
{
	//if multiplier is non-positive
	if(Multiplier<=0)
	{
		//make color black
		CColor c(0,0,0);
		(*this)=c;
	}
	else
	{
		//temp var
		int temp;
		//add red
		temp=(getR()*Multiplier);
		setR(temp);

		//add green
		temp=(getG()*Multiplier);
		setG(temp);

		//add blue
		temp=(getB()*Multiplier);
		setB(temp);
	}
	return(*this);
}

CColor& CColor::operator/=(int Divisor)
{
	//if divisor is non-positive
	if(Divisor<=0)
	{
		//make color black
		CColor c(0,0,0);
		(*this)=c;
	}
	else
	{
		//temp var
		int temp;
		//add red
		temp=(getR()/Divisor);
		setR(temp);

		//add green
		temp=(getG()/Divisor);
		setG(temp);

		//add blue
		temp=(getB()/Divisor);
		setB(temp);
	}
	return(*this);
}

CColor& CColor::operator|=(CColor& Color)
{
	//do red
	setR(getR()|Color.getR());
	//do green
	setG(getG()|Color.getG());
	//do blue
	setB(getB()|Color.getB());
	//return
	return(*this);
}

CColor& CColor::operator&=(CColor& Color)
{
	//do red
	setR(getR()&Color.getR());
	//do green
	setG(getG()&Color.getG());
	//do blue
	setB(getB()&Color.getB());
	//return
	return(*this);
}

CColor& CColor::operator^=(CColor& Color)
{
	//do red
	setR(getR()^Color.getR());
	//do green
	setG(getG()^Color.getG());
	//do blue
	setB(getB()^Color.getB());
	//return
	return(*this);
}

//primary colors
CColor CColor::Red(Uint8 shade)
{
	//make color
	return(CColor(shade,0,0));
}

CColor CColor::Green(Uint8 shade)
{
	//make color
	return(CColor(0,shade,0));
}

CColor CColor::Blue(Uint8 shade)
{
	//make color
	return(CColor(0,0,shade));
}

//secondary colors
CColor CColor::Yellow(Uint8 shade)
{
	//make color
	CColor r = Red(shade);
	CColor g = Green(shade);
	return(r|g);
}

CColor CColor::Cyan(Uint8 shade)
{
	//make color
	CColor b = Blue(shade);
	CColor g = Green(shade);
	return(g|b);
}

CColor CColor::Magenta(Uint8 shade)
{
	CColor r = Red(shade);
	CColor b = Blue(shade);
	return(r|b);
}

//dark colors
CColor CColor::DarkRed(Uint8 shade)
{
	//make color
	return(Red(shade));
}

CColor CColor::DarkGreen(Uint8 shade)
{
	//make color
	return(Green(shade));
}

CColor CColor::DarkBlue(Uint8 shade)
{
	//make color
	return(Blue(shade));
}

CColor CColor::DarkYellow(Uint8 shade)
{
	//make color
	return(Yellow(shade));
}

CColor CColor::DarkCyan(Uint8 shade)
{
	//make color
	return(Cyan(shade));
}

CColor CColor::DarkMagenta(Uint8 shade)
{
	//make color
	return(Magenta(shade));
}

//light colors
CColor CColor::LightRed(Uint8 gray,Uint8 shade)
{
	//make color
	CColor r = Red(shade);
	CColor w = White(gray);
	return(r|w);
}

CColor CColor::LightGreen(Uint8 gray,Uint8 shade)
{
	//make color
	CColor w = White(gray);
	CColor g = Green(shade);
	return(w|g);
}

CColor CColor::LightBlue(Uint8 gray,Uint8 shade)
{
	//make color
	CColor w = White(gray);
	CColor b = Blue(shade);
	return(b|w);
}

CColor CColor::LightYellow(Uint8 gray,Uint8 shade)
{
	//make color
	CColor w = White(gray);
	CColor y = Yellow(shade);
	return(w|y);
}

CColor CColor::LightCyan(Uint8 gray,Uint8 shade)
{
	//make color
	CColor w = White(gray);
	CColor c = Cyan(shade);
	return(c|w);
}

CColor CColor::LightMagenta(Uint8 gray,Uint8 shade)
{
	//make color
	CColor w = White(gray);
	CColor m = Magenta(shade);
	return(m|w);
}

//grayscale
CColor CColor::White(Uint8 shade)
{
	//make color
	CColor r = Red(shade);
	CColor g = Green(shade);
	CColor b = Blue(shade);
	CColor m = r|b;
	return(m|g);
}

CColor CColor::LightGray(Uint8 shade)
{
	//make color
	return(White(shade));
}

CColor CColor::DarkGray(Uint8 shade)
{
	//make color
	return(White(shade));
}

CColor CColor::Black(Uint8 shade)
{
	//make color
	return(White(shade));
}

//arithmetic operators
CColor operator+(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3+=Color2;
	//return color
	return(Color3);
}

CColor operator-(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3-=Color2;
	//return color
	return(Color3);
}

CColor operator*(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3*=Color2;
	//return color
	return(Color3);
}

CColor operator*(CColor& Color,int Multiplier)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color;
	//do operator with color2
	Color3*=Multiplier;
	//return color
	return(Color3);
}

CColor operator/(CColor& Color,int Divisor)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color;
	//do operator with color2
	Color3/=Divisor;
	//return color
	return(Color3);
}

//bitwise operators
CColor operator|(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3|=Color2;
	//return color
	return(Color3);
}

CColor operator&(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3&=Color2;
	//return color
	return(Color3);
}

CColor operator^(CColor& Color1,CColor& Color2)
{
	//temp color
	CColor Color3;
	//assign color1
	Color3=Color1;
	//do operator with color2
	Color3^=Color2;
	//return color
	return(Color3);
}

CColor operator~(CColor& Color)
{
	//temp color
	CColor Color3;
	//assign
	Color3=Color;
	//do operator
	CColor Color2(255,255,255);
	Color3^=Color2;
	//return color
	return(Color3);
}

//comparison operators
bool operator==(CColor& Color1,CColor& Color2)
{
	//do comparison
	return(Color1.getR()==Color2.getR()&&Color1.getG()==Color2.getG()&&Color1.getB()==Color2.getB());
}

bool operator!=(CColor& Color1,CColor& Color2)
{
	//do comparison
	return(!(Color1==Color2));
}

