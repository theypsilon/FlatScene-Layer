#include "FSColor.h"
//standard constructor
FSColor::FSColor(Uint8 r,Uint8 g,Uint8 b)
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
FSColor::FSColor(const FSColor& Color)
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
FSColor::~FSColor()
{
}

//get rgb
Uint8 FSColor::getR() const
{
    //return red
    return(m_Color.r);
}

Uint8 FSColor::getG() const
{
    //return green
    return(m_Color.g);
}

Uint8 FSColor::getB() const
{
    //return blue
    return(m_Color.b);
}

//set rgb
void FSColor::setR(Uint8 r)
{
    //set red
    m_Color.r=r;
}

void FSColor::setG(Uint8 g)
{
    //set green
    m_Color.g=g;
}

void FSColor::setB(Uint8 b)
{
    //set blue
    m_Color.b=b;
}

//rgb accessors
Uint8& FSColor::R()
{
    //return red
    return(m_Color.r);
}

Uint8& FSColor::G()
{
    //return green
    return(m_Color.g);
}

Uint8& FSColor::B()
{
    //return blue
    return(m_Color.b);
}

//conversion operators
FSColor::operator SDL_Color()
{
    //return color
    return(m_Color);
}

FSColor::operator SDL_Color*()
{
    //return pointer to color
    return(&m_Color);
}

//assignment operators
FSColor& FSColor::operator=(FSColor& Color)
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

FSColor& FSColor::operator+=(FSColor& Color)
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

FSColor& FSColor::operator-=(FSColor& Color)
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

FSColor& FSColor::operator*=(FSColor& Color)
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

FSColor& FSColor::operator*=(int Multiplier)
{
    //if multiplier is non-positive
    if(Multiplier<=0)
    {
        //make color black
        FSColor c(0,0,0);
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

FSColor& FSColor::operator/=(int Divisor)
{
    //if divisor is non-positive
    if(Divisor<=0)
    {
        //make color black
        FSColor c(0,0,0);
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

FSColor& FSColor::operator|=(FSColor& Color)
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

FSColor& FSColor::operator&=(FSColor& Color)
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

FSColor& FSColor::operator^=(FSColor& Color)
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
FSColor FSColor::Red(Uint8 shade)
{
    //make color
    return(FSColor(shade,0,0));
}

FSColor FSColor::Green(Uint8 shade)
{
    //make color
    return(FSColor(0,shade,0));
}

FSColor FSColor::Blue(Uint8 shade)
{
    //make color
    return(FSColor(0,0,shade));
}

//secondary colors
FSColor FSColor::Yellow(Uint8 shade)
{
    //make color
    FSColor r = Red(shade);
    FSColor g = Green(shade);
    return(r|g);
}

FSColor FSColor::Cyan(Uint8 shade)
{
    //make color
    FSColor b = Blue(shade);
    FSColor g = Green(shade);
    return(g|b);
}

FSColor FSColor::Magenta(Uint8 shade)
{
    FSColor r = Red(shade);
    FSColor b = Blue(shade);
    return(r|b);
}

//dark colors
FSColor FSColor::DarkRed(Uint8 shade)
{
    //make color
    return(Red(shade));
}

FSColor FSColor::DarkGreen(Uint8 shade)
{
    //make color
    return(Green(shade));
}

FSColor FSColor::DarkBlue(Uint8 shade)
{
    //make color
    return(Blue(shade));
}

FSColor FSColor::DarkYellow(Uint8 shade)
{
    //make color
    return(Yellow(shade));
}

FSColor FSColor::DarkCyan(Uint8 shade)
{
    //make color
    return(Cyan(shade));
}

FSColor FSColor::DarkMagenta(Uint8 shade)
{
    //make color
    return(Magenta(shade));
}

//light colors
FSColor FSColor::LightRed(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor r = Red(shade);
    FSColor w = White(gray);
    return(r|w);
}

FSColor FSColor::LightGreen(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor w = White(gray);
    FSColor g = Green(shade);
    return(w|g);
}

FSColor FSColor::LightBlue(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor w = White(gray);
    FSColor b = Blue(shade);
    return(b|w);
}

FSColor FSColor::LightYellow(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor w = White(gray);
    FSColor y = Yellow(shade);
    return(w|y);
}

FSColor FSColor::LightCyan(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor w = White(gray);
    FSColor c = Cyan(shade);
    return(c|w);
}

FSColor FSColor::LightMagenta(Uint8 gray,Uint8 shade)
{
    //make color
    FSColor w = White(gray);
    FSColor m = Magenta(shade);
    return(m|w);
}

//grayscale
FSColor FSColor::White(Uint8 shade)
{
    //make color
    FSColor r = Red(shade);
    FSColor g = Green(shade);
    FSColor b = Blue(shade);
    FSColor m = r|b;
    return(m|g);
}

FSColor FSColor::LightGray(Uint8 shade)
{
    //make color
    return(White(shade));
}

FSColor FSColor::DarkGray(Uint8 shade)
{
    //make color
    return(White(shade));
}

FSColor FSColor::Black(Uint8 shade)
{
    //make color
    return(White(shade));
}

//arithmetic operators
FSColor operator+(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3+=Color2;
    //return color
    return(Color3);
}

FSColor operator-(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3-=Color2;
    //return color
    return(Color3);
}

FSColor operator*(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3*=Color2;
    //return color
    return(Color3);
}

FSColor operator*(FSColor& Color,int Multiplier)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color;
    //do operator with color2
    Color3*=Multiplier;
    //return color
    return(Color3);
}

FSColor operator/(FSColor& Color,int Divisor)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color;
    //do operator with color2
    Color3/=Divisor;
    //return color
    return(Color3);
}

//bitwise operators
FSColor operator|(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3|=Color2;
    //return color
    return(Color3);
}

FSColor operator&(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3&=Color2;
    //return color
    return(Color3);
}

FSColor operator^(FSColor& Color1,FSColor& Color2)
{
    //temp color
    FSColor Color3;
    //assign color1
    Color3=Color1;
    //do operator with color2
    Color3^=Color2;
    //return color
    return(Color3);
}

FSColor operator~(FSColor& Color)
{
    //temp color
    FSColor Color3;
    //assign
    Color3=Color;
    //do operator
    FSColor Color2(255,255,255);
    Color3^=Color2;
    //return color
    return(Color3);
}

//comparison operators
bool operator==(FSColor& Color1,FSColor& Color2)
{
    //do comparison
    return(Color1.getR()==Color2.getR()&&Color1.getG()==Color2.getG()&&Color1.getB()==Color2.getB());
}

bool operator!=(FSColor& Color1,FSColor& Color2)
{
    //do comparison
    return(!(Color1==Color2));
}

