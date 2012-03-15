#include "FSColor.h"

namespace flatscene {

//standard constructor
Color::Color(Uint8 r,Uint8 g,Uint8 b)
{
    //set red
    setR(r);
    //get green
    setG(g);
    //set blue
    setB(b);
    //set unused to 0
    m_color.unused=0;
}

//copy constructor
Color::Color(const Color& color)
{
    //set red
    setR(color.getR());
    //set green
    setG(color.getG());
    //set blue
    setB(color.getB());
    //set unused to 0
    m_color.unused=0;
}

//destructor
Color::~Color()
{
}

//get rgb
Uint8 Color::getR() const
{
    //return red
    return(m_color.r);
}

Uint8 Color::getG() const
{
    //return green
    return(m_color.g);
}

Uint8 Color::getB() const
{
    //return blue
    return(m_color.b);
}

//set rgb
void Color::setR(Uint8 r)
{
    //set red
    m_color.r=r;
}

void Color::setG(Uint8 g)
{
    //set green
    m_color.g=g;
}

void Color::setB(Uint8 b)
{
    //set blue
    m_color.b=b;
}

//rgb accessors
Uint8& Color::R()
{
    //return red
    return(m_color.r);
}

Uint8& Color::G()
{
    //return green
    return(m_color.g);
}

Uint8& Color::B()
{
    //return blue
    return(m_color.b);
}

//conversion operators
Color::operator SDL_Color()
{
    //return color
    return(m_color);
}

Color::operator SDL_Color*()
{
    //return pointer to color
    return(&m_color);
}

//assignment operators
Color& Color::operator=(Color& color)
{
    //set red
    setR(color.getR());
    //set green
    setG(color.getG());
    //set blue
    setB(color.getB());
    //return
    return(*this);
}

Color& Color::operator+=(Color& color)
{
    //temp var
    int temp;
    //add red
    temp=getR()+color.getR();
    if(temp>255) temp=255;
    setR(temp);

    //add green
    temp=getG()+color.getG();
    if(temp>255) temp=255;
    setG(temp);

    //add blue
    temp=getB()+color.getB();
    if(temp>255) temp=255;
    setB(temp);

    //return
    return(*this);
}

Color& Color::operator-=(Color& color)
{
    //temp var
    int temp;
    //add red
    temp=getR()-color.getR();
    if(temp<0) temp=0;
    setR(temp);

    //add green
    temp=getG()-color.getG();
    if(temp<0) temp=0;
    setG(temp);

    //add blue
    temp=getB()-color.getB();
    if(temp<0) temp=0;
    setB(temp);

    //return
    return(*this);
}

Color& Color::operator*=(Color& color)
{
    //temp var
    int temp;
    //add red
    temp=(getR()*color.getR())/255;
    setR(temp);

    //add green
    temp=(getG()*color.getG())/255;
    setG(temp);

    //add blue
    temp=(getB()*color.getB())/255;
    setB(temp);

    //return
    return(*this);
}

Color& Color::operator*=(int multiplier)
{
    //if multiplier is non-positive
    if(multiplier<=0)
    {
        //make color black
        Color c(0,0,0);
        (*this)=c;
    }
    else
    {
        //temp var
        int temp;
        //add red
        temp=(getR()*multiplier);
        setR(temp);

        //add green
        temp=(getG()*multiplier);
        setG(temp);

        //add blue
        temp=(getB()*multiplier);
        setB(temp);
    }
    return(*this);
}

Color& Color::operator/=(int divisor)
{
    //if divisor is non-positive
    if(divisor<=0)
    {
        //make color black
        Color c(0,0,0);
        (*this)=c;
    }
    else
    {
        //temp var
        int temp;
        //add red
        temp=(getR()/divisor);
        setR(temp);

        //add green
        temp=(getG()/divisor);
        setG(temp);

        //add blue
        temp=(getB()/divisor);
        setB(temp);
    }
    return(*this);
}

Color& Color::operator|=(Color& color)
{
    //do red
    setR(getR()|color.getR());
    //do green
    setG(getG()|color.getG());
    //do blue
    setB(getB()|color.getB());
    //return
    return(*this);
}

Color& Color::operator&=(Color& color)
{
    //do red
    setR(getR()&color.getR());
    //do green
    setG(getG()&color.getG());
    //do blue
    setB(getB()&color.getB());
    //return
    return(*this);
}

Color& Color::operator^=(Color& color)
{
    //do red
    setR(getR()^color.getR());
    //do green
    setG(getG()^color.getG());
    //do blue
    setB(getB()^color.getB());
    //return
    return(*this);
}

//primary colors
Color Color::Red(Uint8 shade)
{
    //make color
    return(Color(shade,0,0));
}

Color Color::Green(Uint8 shade)
{
    //make color
    return(Color(0,shade,0));
}

Color Color::Blue(Uint8 shade)
{
    //make color
    return(Color(0,0,shade));
}

//secondary colors
Color Color::Yellow(Uint8 shade)
{
    //make color
    Color r = Red(shade);
    Color g = Green(shade);
    return(r|g);
}

Color Color::Cyan(Uint8 shade)
{
    //make color
    Color b = Blue(shade);
    Color g = Green(shade);
    return(g|b);
}

Color Color::Magenta(Uint8 shade)
{
    Color r = Red(shade);
    Color b = Blue(shade);
    return(r|b);
}

//dark colors
Color Color::DarkRed(Uint8 shade)
{
    //make color
    return(Red(shade));
}

Color Color::DarkGreen(Uint8 shade)
{
    //make color
    return(Green(shade));
}

Color Color::DarkBlue(Uint8 shade)
{
    //make color
    return(Blue(shade));
}

Color Color::DarkYellow(Uint8 shade)
{
    //make color
    return(Yellow(shade));
}

Color Color::DarkCyan(Uint8 shade)
{
    //make color
    return(Cyan(shade));
}

Color Color::DarkMagenta(Uint8 shade)
{
    //make color
    return(Magenta(shade));
}

//light colors
Color Color::LightRed(Uint8 gray,Uint8 shade)
{
    //make color
    Color r = Red(shade);
    Color w = White(gray);
    return(r|w);
}

Color Color::LightGreen(Uint8 gray,Uint8 shade)
{
    //make color
    Color w = White(gray);
    Color g = Green(shade);
    return(w|g);
}

Color Color::LightBlue(Uint8 gray,Uint8 shade)
{
    //make color
    Color w = White(gray);
    Color b = Blue(shade);
    return(b|w);
}

Color Color::LightYellow(Uint8 gray,Uint8 shade)
{
    //make color
    Color w = White(gray);
    Color y = Yellow(shade);
    return(w|y);
}

Color Color::LightCyan(Uint8 gray,Uint8 shade)
{
    //make color
    Color w = White(gray);
    Color c = Cyan(shade);
    return(c|w);
}

Color Color::LightMagenta(Uint8 gray,Uint8 shade)
{
    //make color
    Color w = White(gray);
    Color m = Magenta(shade);
    return(m|w);
}

//grayscale
Color Color::White(Uint8 shade)
{
    //make color
    Color r = Red(shade);
    Color g = Green(shade);
    Color b = Blue(shade);
    Color m = r|b;
    return(m|g);
}

Color Color::LightGray(Uint8 shade)
{
    //make color
    return(White(shade));
}

Color Color::DarkGray(Uint8 shade)
{
    //make color
    return(White(shade));
}

Color Color::Black(Uint8 shade)
{
    //make color
    return(White(shade));
}

//arithmetic operators
Color operator+(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3+=color2;
    //return color
    return(color3);
}

Color operator-(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3-=color2;
    //return color
    return(color3);
}

Color operator*(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3*=color2;
    //return color
    return(color3);
}

Color operator*(Color& color,int multiplier)
{
    //temp color
    Color color3;
    //assign color1
    color3=color;
    //do operator with color2
    color3*=multiplier;
    //return color
    return(color3);
}

Color operator/(Color& color,int divisor)
{
    //temp color
    Color color3;
    //assign color1
    color3=color;
    //do operator with color2
    color3/=divisor;
    //return color
    return(color3);
}

//bitwise operators
Color operator|(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3|=color2;
    //return color
    return(color3);
}

Color operator&(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3&=color2;
    //return color
    return(color3);
}

Color operator^(Color& color1,Color& color2)
{
    //temp color
    Color color3;
    //assign color1
    color3=color1;
    //do operator with color2
    color3^=color2;
    //return color
    return(color3);
}

Color operator~(Color& color)
{
    //temp color
    Color color3;
    //assign
    color3=color;
    //do operator
    Color color2(255,255,255);
    color3^=color2;
    //return color
    return(color3);
}

//comparison operators
bool operator==(Color& color1,Color& color2)
{
    //do comparison
    return(color1.getR()==color2.getR()&&color1.getG()==color2.getG()&&color1.getB()==color2.getB());
}

bool operator!=(Color& color1,Color& color2)
{
    //do comparison
    return(!(color1==color2));
}

} // flatscene
