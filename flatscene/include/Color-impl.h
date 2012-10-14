#ifndef FS_COLOR_IMPL_H__
#define FS_COLOR_IMPL_H__

#include <stdexcept>

namespace FlatScene {

namespace detail {

    template <RGBA::RGBA C> Byte constexpr bsh() {
        return  (C == RGBA::r)? 24 :
                (C == RGBA::g)? 16 :
                (C == RGBA::b)? 8  :
                /*          a*/ 0  ;
    }

    template <RGBA::RGBA C> Byte constexpr bad() {
        return  (C == RGBA::r)? 3 :
                (C == RGBA::g)? 2 :
                (C == RGBA::b)? 1 :
                /*          a*/ 0 ;
    }

    template <RGBA::RGBA C = RGBA::a> constexpr unsigned int mask() {
        return  (C == RGBA::r)? 0xFF << bsh<C>() :
                (C == RGBA::g)? 0xFF << bsh<C>() :
                (C == RGBA::b)? 0xFF << bsh<C>() :
                /*          a*/ 0xFF << bsh<C>() ;
    }

    constexpr inline Byte ftob(Float n) {
        return n >= 1.0?  255 :
               n <= 0.0?    0 :
               (Byte) n*255.0 ;
    }

    constexpr inline Byte fchar(char c) {
        return c == '0'            ?  0 :
               c == '1'            ?  1 :
               c == '2'            ?  2 :
               c == '3'            ?  3 :
               c == '4'            ?  4 :
               c == '5'            ?  5 :
               c == '6'            ?  6 : 
               c == '7'            ?  7 :
               c == '8'            ?  8 :
               c == '9'            ?  9 :
               c == 'A' || c == 'a'? 10 :
               c == 'B' || c == 'b'? 11 :
               c == 'C' || c == 'c'? 12 :
               c == 'D' || c == 'd'? 13 :
               c == 'E' || c == 'e'? 14 :
               c == 'F' || c == 'f'? 15 :
               throw std::logic_error("color string contain wrong symbols.");
    }

    constexpr inline unsigned int fstr(std::size_t n,const char a[]) { 
        return  n == 10                                                          || 
                n == 8? a[0] == '#'? fstr(n-1,a+1)                                :
                        throw std::logic_error("color str is ill-formed, use hex"):
                n == 9? fstr(7,a)         | fchar(a[6]) <<  4 | fchar(a[7]) <<  0 :
                n == 7? fchar(a[0]) << 28 | fchar(a[1]) << 24 | fchar(a[2]) << 20 |  
                        fchar(a[3]) << 16 | fchar(a[4]) << 12 | fchar(a[5]) <<  8 :
                throw std::logic_error("color str may only be 8/6 symbols")       ;
    }

} //detail

constexpr Color::Color(unsigned int hex, bool alpha) noexcept
    : _color(alpha? hex : hex << 8)
{}

constexpr Color::Color(Byte nr,Byte ng,Byte nb, Byte na) noexcept
    : _color((nr << detail::bsh<RGBA::r>()) | 
             (ng << detail::bsh<RGBA::g>()) | 
             (nb << detail::bsh<RGBA::b>()) | 
             (na << detail::bsh<RGBA::a>()) )
{}

template <std::size_t N> constexpr Color::Color(const char(&a)[N])
    : _color(detail::fstr(N,a))
{}

inline Color::Color(const std::string& a)
    : _color(detail::fstr(a.size()+1,a.c_str()))
{}

constexpr Color::Color(const Color& color) noexcept
    : _color(color._color)
{}

template <RGBA::RGBA C> constexpr inline Byte Color::get() const {
    return (Byte)((_color & detail::mask<C>()) >> detail::bsh<C>());
}

template <RGBA::RGBA C> inline Color& Color::set(Byte c) {
    _color = (_color & ~detail::mask<C>()) | (c << detail::bsh<C>());
    return *this;
}

template <RGBA::RGBA C> inline Byte& Color::ref() {
    return *(reinterpret_cast<Byte*>(&_color)+detail::bad<C>());
}

constexpr inline Byte  Color::getR() const { return get<RGBA::r>(); }
constexpr inline Byte  Color::getG() const { return get<RGBA::g>(); }
constexpr inline Byte  Color::getB() const { return get<RGBA::b>(); }
constexpr inline Byte  Color::getA() const { return get<RGBA::a>(); }
constexpr inline unsigned int Color::getHex() const { return _color; }

inline Color& Color::setR(Byte c) { return set<RGBA::r>(c); }
inline Color& Color::setG(Byte c) { return set<RGBA::g>(c); }
inline Color& Color::setB(Byte c) { return set<RGBA::b>(c); }
inline Color& Color::setA(Byte c) { return set<RGBA::a>(c); }

inline Byte& Color::R() { return ref<RGBA::r>(); }
inline Byte& Color::G() { return ref<RGBA::g>(); }
inline Byte& Color::B() { return ref<RGBA::b>(); }
inline Byte& Color::A() { return ref<RGBA::a>(); }

inline Color& Color::operator=(const Color& color) noexcept {
    _color = color._color;
    return *this;
}

inline Color& Color::operator-=(const Color& color) { _color |= ~color.getHex(); return *this; }
inline Color& Color::operator|=(const Color& color) { _color |=  color.getHex(); return *this; }
inline Color& Color::operator&=(const Color& color) { _color &=  color.getHex(); return *this; }
inline Color& Color::operator^=(const Color& color) { _color ^=  color.getHex(); return *this; }
inline Color& Color::operator+=(const Color& color) { return operator|=(color); }

constexpr inline Color FColor(Float nr,Float ng,Float nb, Float na) {
    return Color(detail::ftob(nr), detail::ftob(ng), detail::ftob(nb), detail::ftob(na));
}

constexpr inline Color Red         (Byte shade)           { return Color(shade,0,0); }
constexpr inline Color Green       (Byte shade)           { return Color(0,shade,0); }
constexpr inline Color Blue        (Byte shade)           { return Color(0,0,shade); }

constexpr inline Color Yellow      (Byte shade)           { return Red  (shade) |Green(shade); }
constexpr inline Color Cyan        (Byte shade)           { return Green(shade) |Blue (shade); }
constexpr inline Color Magenta     (Byte shade)           { return Red  (shade) |Blue (shade); }

constexpr inline Color DarkRed     (Byte shade)           { return Red    (shade); }
constexpr inline Color DarkGreen   (Byte shade)           { return Green  (shade); }
constexpr inline Color DarkBlue    (Byte shade)           { return Blue   (shade); }
constexpr inline Color DarkYellow  (Byte shade)           { return Yellow (shade); }
constexpr inline Color DarkCyan    (Byte shade)           { return Cyan   (shade); }
constexpr inline Color DarkMagenta (Byte shade)           { return Magenta(shade); }

constexpr inline Color LightRed    (Byte gray,Byte shade) { return Red(shade)     | White(gray); }
constexpr inline Color LightGreen  (Byte gray,Byte shade) { return Green(shade)   | White(gray); }
constexpr inline Color LightBlue   (Byte gray,Byte shade) { return Blue(shade)    | White(gray); }
constexpr inline Color LightYellow (Byte gray,Byte shade) { return Yellow(shade)  | White(gray); }
constexpr inline Color LightCyan   (Byte gray,Byte shade) { return Cyan(shade)    | White(gray); }
constexpr inline Color LightMagenta(Byte gray,Byte shade) { return Magenta(shade) | White(gray); }

constexpr inline Color White       (Byte shade)           { return Color(shade,shade,shade); }
constexpr inline Color Gray        (Byte shade)           { return White(shade); }
constexpr inline Color DarkGray    (Byte shade)           { return White(shade); }
constexpr inline Color Black       (Byte shade)           { return White(shade); }


inline Color operator+(Color color1,const Color& color2)  { return color1 += color2; }
inline Color operator-(Color color1,const Color& color2)  { return color1 -= color2; }
inline Color operator&(Color color1,const Color& color2)  { return color1 &= color2; }
inline Color operator^(Color color1,const Color& color2)  { return color1 ^= color2; }

constexpr inline Color operator~(const Color& color)      { return Color(~color.getHex()); }
constexpr inline Color operator|(const Color& color1,const Color& color2)
                                { return Color(color1.getHex() | color2.getHex()); }

constexpr inline bool operator!=(const Color& color1,const Color& color2) { return !(color1==color2); }
constexpr inline bool operator==(const Color& color1,const Color& color2) {
    return color1.getHex() == color2.getHex();
}

} // flatscene


#endif