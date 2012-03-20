#include "FSColor.h"

namespace flatscene {

Color::Color(Byte r,Byte g,Byte b, Byte a)
    : mr(r), mg(g), mb(b), ma(a)
{}

Color::Color(const Color& color)
    : mr(color.mr), mg(color.mg), mb(color.mb), ma(color.ma)
{}

Color::~Color() {}

Byte Color::getR() const { return(mr); }
Byte Color::getG() const { return(mg); }
Byte Color::getB() const { return(mb); }
Byte Color::getA() const { return(ma); }

void Color::setR(Byte r) { mr=r; }
void Color::setG(Byte g) { mg=g; }
void Color::setB(Byte b) { mb=b; }
void Color::setA(Byte a) { ma=a; }

Byte& Color::R() { return(mr); }
Byte& Color::G() { return(mg); }
Byte& Color::B() { return(mb); }
Byte& Color::A() { return(ma); }

Color& Color::operator=(Color& color) {
    setR(color.getR());
    setG(color.getG());
    setB(color.getB());

    return(*this);
}

Color& Color::operator+=(Color& color) {

    int temp;

    temp=getR()+color.getR();
    if(temp>255) temp=255;
    setR(temp);

    temp=getG()+color.getG();
    if(temp>255) temp=255;
    setG(temp);

    temp=getB()+color.getB();
    if(temp>255) temp=255;
    setB(temp);

    return(*this);
}

Color& Color::operator-=(Color& color) {

    int temp;

    temp=getR()-color.getR();
    if(temp<0) temp=0;
    setR(temp);

    temp=getG()-color.getG();
    if(temp<0) temp=0;
    setG(temp);

    temp=getB()-color.getB();
    if(temp<0) temp=0;
    setB(temp);

    return(*this);
}

Color& Color::operator*=(Color& color) {

    int temp;

    temp=(getR()*color.getR())/255;
    setR(temp);

    temp=(getG()*color.getG())/255;
    setG(temp);

    temp=(getB()*color.getB())/255;
    setB(temp);

    return(*this);
}

Color& Color::operator*=(int multiplier) {

    if(multiplier<=0) {
        (*this)=Color(0,0,0);
    } else {
        int temp;

        temp=(getR()*multiplier);
        setR(temp);

        temp=(getG()*multiplier);
        setG(temp);

        temp=(getB()*multiplier);
        setB(temp);
    }
    return(*this);
}

Color& Color::operator/=(int divisor) {
    if(divisor<=0) {
        (*this)=Color(0,0,0);
    } else {
        setR(getR()/divisor);
        setG(getG()/divisor);
        setB(getB()/divisor);
    }

    return(*this);
}

Color& Color::operator|=(Color& color) {

    setR(getR()|color.getR());
    setG(getG()|color.getG());
    setB(getB()|color.getB());

    return(*this);
}

Color& Color::operator&=(Color& color) {

    setR(getR()&color.getR());
    setG(getG()&color.getG());
    setB(getB()&color.getB());

    return(*this);
}

Color& Color::operator^=(Color& color) {

    setR(getR()^color.getR());
    setG(getG()^color.getG());
    setB(getB()^color.getB());

    return(*this);
}

Color Color::Red(Byte shade)        { return(Color(shade,0,0)); }
Color Color::Green(Byte shade)      { return(Color(0,shade,0)); }
Color Color::Blue(Byte shade)       { return(Color(0,0,shade)); }

Color Color::Yellow(Byte shade)     { return(Red(shade)|Green(shade)); }
Color Color::Cyan(Byte shade)       { return(Green(shade)|Blue(shade)); }
Color Color::Magenta(Byte shade)    { return(Red(shade)|Blue(shade)); }

Color Color::LightRed(Byte gray,Byte shade)     { return (Red(shade)|White(gray)); }
Color Color::LightGreen(Byte gray,Byte shade)   { return (Green(shade)|White(gray)); }
Color Color::LightBlue(Byte gray,Byte shade)    { return (Blue(shade)|White(gray)); }
Color Color::LightYellow(Byte gray,Byte shade)  { return (Yellow(shade)|White(gray)); }
Color Color::LightCyan(Byte gray,Byte shade)    { return (Cyan(shade)|White(gray)); }
Color Color::LightMagenta(Byte gray,Byte shade) { return (Magenta(shade)|White(gray)); }

Color Color::White()                { return(Color(shade,shade,shade)); }
Color Color::LightGray(Byte shade)  { return(White(shade)); }
Color Color::DarkGray(Byte shade)   { return(White(shade)); }
Color Color::Black()                { return(White(shade)); }


Color operator+(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3+=color2;

    return(color3);
}

Color operator-(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3-=color2;

    return(color3);
}

Color operator*(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3*=color2;

    return(color3);
}

Color operator*(Color& color,int multiplier) {

    Color color3;

    color3=color;

    color3*=multiplier;

    return(color3);
}

Color operator/(Color& color,int divisor) {

    Color color3;

    color3=color;

    color3/=divisor;

    return(color3);
}


Color operator|(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3|=color2;

    return(color3);
}

Color operator&(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3&=color2;

    return(color3);
}

Color operator^(Color& color1,Color& color2) {

    Color color3;

    color3=color1;

    color3^=color2;

    return(color3);
}

Color operator~(Color& color) {

    Color color3;

    color3=color;

    Color color2(255,255,255);
    color3^=color2;

    return(color3);
}

bool operator==(Color& color1,Color& color2) {
    return(color1.getR()==color2.getR()&&color1.getG()==color2.getG()&&color1.getB()==color2.getB());
}

bool operator!=(Color& color1,Color& color2) {
    return(!(color1==color2));
}

} // flatscene
