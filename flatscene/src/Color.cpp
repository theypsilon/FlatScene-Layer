#include "Color.h"

namespace FlatScene {

namespace D {

    template <RGBA::RGBA C> Byte constexpr bsh() {
        return  (C == RGBA::r)? 24 :
                (C == RGBA::g)? 16 :
                (C == RGBA::b)? 8  :
                                0  ;
    }

    template <RGBA::RGBA C> Byte constexpr bad() {
        return  (C == RGBA::r)? 3 :
                (C == RGBA::g)? 2 :
                (C == RGBA::b)? 1 :
                                0 ;
    }

    template <RGBA::RGBA C = RGBA::a> constexpr unsigned int mask() {
        return  (C == RGBA::r)? 0xFF << bsh<C>() :
                (C == RGBA::g)? 0xFF << bsh<C>() :
                (C == RGBA::b)? 0xFF << bsh<C>() :
                                0xFF << bsh<C>() ;
    }

    template <RGBA::RGBA C> constexpr inline Byte gcol(unsigned int color) {
        return (Byte)((color & mask<C>()) >> bsh<C>());
    }

    template <RGBA::RGBA C> constexpr inline unsigned int scol(unsigned int color, Byte c) {
        return (color & ~mask<C>()) | c << bsh<C>();
    }

    template <RGBA::RGBA C> inline Byte& rcol(unsigned int& color) {
        return *(reinterpret_cast<Byte*>(&color)+bad<C>());
    }

} //D

using namespace D;

Color::Color(unsigned int hex)
    : _color(hex)
{}

Color::Color(Byte r,Byte g,Byte b, Byte a)
    : _color((r << bsh<RGBA::r>()) | 
             (g << bsh<RGBA::g>()) | 
             (b << bsh<RGBA::b>()) | 
             (a << bsh<RGBA::a>()) )
{}

Color::Color(const Color& color)
    : _color(color._color)
{}

Byte Color::getR() const { return gcol<RGBA::r>(_color); }
Byte Color::getG() const { return gcol<RGBA::g>(_color); }
Byte Color::getB() const { return gcol<RGBA::b>(_color); }
Byte Color::getA() const { return gcol<RGBA::a>(_color); }
unsigned int Color::getHex() const { return _color; }

void Color::setR(Byte c) { _color = scol<RGBA::r>(_color,c); }
void Color::setG(Byte c) { _color = scol<RGBA::g>(_color,c); }
void Color::setB(Byte c) { _color = scol<RGBA::b>(_color,c); }
void Color::setA(Byte c) { _color = scol<RGBA::a>(_color,c); }

Byte& Color::R() { return rcol<RGBA::r>(_color); }
Byte& Color::G() { return rcol<RGBA::g>(_color); }
Byte& Color::B() { return rcol<RGBA::b>(_color); }
Byte& Color::A() { return rcol<RGBA::a>(_color); }

Color& Color::operator=(const Color& color) {
    _color = color._color;
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
