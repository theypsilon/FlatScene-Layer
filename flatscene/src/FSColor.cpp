#include "FSColor.h"

namespace flatscene {

Color::Color(Byte r,Byte g,Byte b, Byte a)
    : mr(r), mg(g), mb(b), ma(a)
{}

Color::Color(const Color& color)
    : mr(color.mr), mg(color.mg), mb(color.mb), ma(color.ma)
{}

Color::~Color() {}

Byte Color::getR() const { return mr; }
Byte Color::getG() const { return mg; }
Byte Color::getB() const { return mb; }
Byte Color::getA() const { return ma; }

void Color::setR(Byte r) { mr=r; }
void Color::setG(Byte g) { mg=g; }
void Color::setB(Byte b) { mb=b; }
void Color::setA(Byte a) { ma=a; }

Byte& Color::R() { return mr; }
Byte& Color::G() { return mg; }
Byte& Color::B() { return mb; }
Byte& Color::A() { return ma; }

Color& Color::operator=(const Color& color) {
    setR(color.getR());
    setG(color.getG());
    setB(color.getB());

    return *this;
}

Color& Color::operator+=(const Color& color) {
    int
    temp = getR() + color.getR();   setR(temp < 255 ? temp : 255);
    temp = getG() + color.getG();   setG(temp < 255 ? temp : 255);
    temp = getB() + color.getB();   setB(temp < 255 ? temp : 255);

    return *this;
}

Color& Color::operator-=(const Color& color) {

    setR(color.getR() < getR() ? getR() - color.getR() : 0);
    setG(color.getG() < getG() ? getG() - color.getG() : 0);
    setB(color.getB() < getB() ? getB() - color.getB() : 0);

    return *this;
}

Color& Color::operator*=(const Color& color) {
    
    setR((getR() * color.getR()) /255);
    setG((getG() * color.getG()) /255);
    setB((getB() * color.getB()) /255);
    
    return *this;
}

Color& Color::operator*=(int multiplier) {

    if(multiplier<=0) {
        *this = Color(0,0,0);
    } else {
        setR(getR() * multiplier);
        setG(getG() * multiplier);
        setB(getB() * multiplier);
    }
    return *this;
}

Color& Color::operator/=(int divisor) {
    if(divisor<=0) {
        *this = Color(0,0,0);
    } else {
        setR(getR() / divisor);
        setG(getG() / divisor);
        setB(getB() / divisor);
    }

    return *this;
}

Color& Color::operator|=(const Color& color) {

    setR(getR() | color.getR());
    setG(getG() | color.getG());
    setB(getB() | color.getB());

    return *this;
}

Color& Color::operator&=(const Color& color) {

    setR(getR() & color.getR());
    setG(getG() & color.getG());
    setB(getB() & color.getB());

    return *this;
}

Color& Color::operator^=(const Color& color) {

    setR(getR() ^ color.getR());
    setG(getG() ^ color.getG());
    setB(getB() ^ color.getB());

    return *this;
}

Color Color::Red(Byte shade)        { return Color(shade,0,0); }
Color Color::Green(Byte shade)      { return Color(0,shade,0); }
Color Color::Blue(Byte shade)       { return Color(0,0,shade); }

Color Color::Yellow(Byte shade)     { return Red(shade)   |Green(shade); }
Color Color::Cyan(Byte shade)       { return Green(shade) |Blue(shade); }
Color Color::Magenta(Byte shade)    { return Red(shade)   |Blue(shade); }

Color Color::LightRed(Byte gray,Byte shade)     { return Red(shade)     | White(gray); }
Color Color::LightGreen(Byte gray,Byte shade)   { return Green(shade)   | White(gray); }
Color Color::LightBlue(Byte gray,Byte shade)    { return Blue(shade)    | White(gray); }
Color Color::LightYellow(Byte gray,Byte shade)  { return Yellow(shade)  | White(gray); }
Color Color::LightCyan(Byte gray,Byte shade)    { return Cyan(shade)    | White(gray); }
Color Color::LightMagenta(Byte gray,Byte shade) { return Magenta(shade) | White(gray); }

Color Color::White(Byte shade)      { return Color(shade,shade,shade); }
Color Color::Gray(Byte shade)       { return White(shade); }
Color Color::DarkGray(Byte shade)   { return White(shade); }
Color Color::Black(Byte shade)      { return White(shade); }


Color operator+(Color color1,const Color& color2) { return color1 += color2; }
Color operator-(Color color1,const Color& color2) { return color1 -= color2; }
Color operator*(Color color1,const Color& color2) { return color1 *= color2; }
Color operator*(Color color,int multiplier)       { return color  *= multiplier; }
Color operator/(Color color,int divisor)          { return color  /= divisor; }
Color operator|(Color color1,const Color& color2) { return color1 |= color2; }
Color operator&(Color color1,const Color& color2) { return color1 &= color2; }
Color operator^(Color color1,const Color& color2) { return color1 ^= color2; }
Color operator~(Color color)                      { return color  ^= Color(255,255,255); }

bool operator!=(const Color& color1,const Color& color2) { return !(color1==color2); }
bool operator==(const Color& color1,const Color& color2) {
    return color1.getR() == color2.getR()
        && color1.getG() == color2.getG()
        && color1.getB() == color2.getB();
}

} // flatscene
