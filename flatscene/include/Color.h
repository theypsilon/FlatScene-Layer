#ifndef FS_COLOR_H__
#define FS_COLOR_H__

#include <cstddef>
#include <string>
#include "Types.h"

namespace FlatScene {

    namespace RGBA {
        enum RGBA { r, g, b, a };
    }

    class Color {
    private:
        unsigned int _color;
    public:

        constexpr Color(Byte nr, Byte ng, Byte nb, Byte na=0);
        constexpr explicit Color(unsigned int hex, bool alpha = true);
        template <std::size_t N> constexpr explicit Color(const char(&a)[N]);
        explicit Color(const std::string& a);
        constexpr Color(const Color& color);

        template <RGBA::RGBA component> constexpr Byte get() const;
        constexpr Byte getR() const;
        constexpr Byte getG() const;
        constexpr Byte getB() const;
        constexpr Byte getA() const;
        constexpr unsigned int getHex() const;

        template <RGBA::RGBA component> Color& set(Byte c);
        Color& setR(Byte c);
        Color& setG(Byte c);
        Color& setB(Byte c);
        Color& setA(Byte c);

        template <RGBA::RGBA component> Byte& ref();
        Byte& R();
        Byte& G();
        Byte& B();
        Byte& A();

        Color& operator=(const Color& color);
        Color& operator+=(const Color& color);
        Color& operator-=(const Color& color);
        Color& operator*=(const Color& color);
        Color& operator*=(int Multiplier);
        Color& operator/=(int Divisor);
        Color& operator|=(const Color& color);
        Color& operator&=(const Color& color);
        Color& operator^=(const Color& color);
    };

    constexpr Color FColor(Float nr, Float ng, Float nb, Float na=0);

    constexpr Color Red(Byte shade=255);
    constexpr Color Green(Byte shade=255);
    constexpr Color Blue(Byte shade=255);

    constexpr Color Yellow(Byte shade=255);
    constexpr Color Cyan(Byte shade=255);
    constexpr Color Magenta(Byte shade=255);

    constexpr Color DarkRed(Byte shade=128);
    constexpr Color DarkGreen(Byte shade=128);
    constexpr Color DarkBlue(Byte shade=128);
    constexpr Color DarkYellow(Byte shade=128);
    constexpr Color DarkCyan(Byte shade=128);
    constexpr Color DarkMagenta(Byte shade=128);

    constexpr Color LightRed(Byte gray=128,Byte shade=255);
    constexpr Color LightGreen(Byte gray=128,Byte shade=255);
    constexpr Color LightBlue(Byte gray=128,Byte shade=255);
    constexpr Color LightYellow(Byte gray=128,Byte shade=255);
    constexpr Color LightCyan(Byte gray=128,Byte shade=255);
    constexpr Color LightMagenta(Byte gray=128,Byte shade=255);

    constexpr Color White(Byte shade=255);
    constexpr Color Gray(Byte shade=192);
    constexpr Color DarkGray(Byte shade=128);
    constexpr Color Black(Byte shade=0);

    Color operator+(Color color1,const Color& color2);
    Color operator-(Color color1,const Color& color2);
    Color operator*(Color color1,const Color& color2);
    Color operator*(Color color,int Multiplier);
    Color operator/(Color color,int Divisor);

    constexpr Color operator|(Color color1,const Color& color2);
    constexpr Color operator~(Color color);
    Color operator&(Color color1,const Color& color2);
    Color operator^(Color color1,const Color& color2);

    constexpr bool operator==(const Color& color1,const Color& color2);
    constexpr bool operator!=(const Color& color1,const Color& color2);

} // flatscene

#include "Color-impl.h"

#endif //#ifndef __COLOR_H__
