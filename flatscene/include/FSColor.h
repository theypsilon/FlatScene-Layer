#ifndef __COLOR_H__
#define __COLOR_H__

#include "FSTypes.h"

namespace flatscene {

    class Color {
    private:
        Byte mr, mg, mb, ma;
    public:

        Color(Byte r=0,Byte g=0,Byte b=0, Byte a=0);
        Color(const Color& Color);
        virtual ~Color();

        Byte getR() const;
        Byte getG() const;
        Byte getB() const;
        Byte getA() const;

        void setR(Byte r);
        void setG(Byte g);
        void setB(Byte b);
        void setA(Byte a);

        Byte& R();
        Byte& G();
        Byte& B();
        Byte& A();

        Color& operator=(Color& Color);
        Color& operator+=(Color& Color);
        Color& operator-=(Color& Color);
        Color& operator*=(Color& Color);
        Color& operator*=(int Multiplier);
        Color& operator/=(int Divisor);
        Color& operator|=(Color& Color);
        Color& operator&=(Color& Color);
        Color& operator^=(Color& Color);

        static Color Red(Byte shade=255);
        static Color Green(Byte shade=255);
        static Color Blue(Byte shade=255);

        static Color Yellow(Byte shade=255);
        static Color Cyan(Byte shade=255);
        static Color Magenta(Byte shade=255);

        static Color DarkRed(Byte shade=128)        { return Red(shade) }
        static Color DarkGreen(Byte shade=128)      { return Green(shade) }
        static Color DarkBlue(Byte shade=128)       { return Blue(shade) }
        static Color DarkYellow(Byte shade=128)     { return Yellow(shade) }
        static Color DarkCyan(Byte shade=128)       { return Cyan(shade) }
        static Color DarkMagenta(Byte shade=128)    { return Magenta(shade) }

        static Color LightRed(Byte gray=128,Byte shade=255);
        static Color LightGreen(Byte gray=128,Byte shade=255);
        static Color LightBlue(Byte gray=128,Byte shade=255);
        static Color LightYellow(Byte gray=128,Byte shade=255);
        static Color LightCyan(Byte gray=128,Byte shade=255);
        static Color LightMagenta(Byte gray=128,Byte shade=255);

        static Color White(Byte shade=255);
        static Color Gray(Byte shade=192);
        static Color DarkGray(Byte shade=128);
        static Color Black(Byte shade=0);
    };

    Color operator+(Color& Color1,Color& Color2);
    Color operator-(Color& Color1,Color& Color2);
    Color operator*(Color& Color1,Color& Color2);
    Color operator*(Color& Color,int Multiplier);
    Color operator/(Color& Color,int Divisor);

    Color operator|(Color& Color1,Color& Color2);
    Color operator&(Color& Color1,Color& Color2);
    Color operator^(Color& Color1,Color& Color2);
    Color operator~(Color& Color);

    bool operator==(Color& Color1,Color& Color2);
    bool operator!=(Color& Color1,Color& Color2);

} // flatscene

#endif //#ifndef __COLOR_H__
