#ifndef __COLOR_H__
#define __COLOR_H__

#include "SDL.h"
#include "FSTypes.h"

namespace flatscene {

    class Color {
    private:
        SDL_Color m_color;
    public:

        Color(Uint8 r=0,Uint8 g=0,Uint8 b=0);
        Color(const Color& Color);
        virtual ~Color();

        Uint8 getR() const;
        Uint8 getG() const;
        Uint8 getB() const;

        void setR(Uint8 r);
        void setG(Uint8 g);
        void setB(Uint8 b);

        Uint8& R();
        Uint8& G();
        Uint8& B();

        explicit operator SDL_Color();
        explicit operator SDL_Color*();

        Color& operator=(Color& Color);
        Color& operator+=(Color& Color);
        Color& operator-=(Color& Color);
        Color& operator*=(Color& Color);
        Color& operator*=(int Multiplier);
        Color& operator/=(int Divisor);
        Color& operator|=(Color& Color);
        Color& operator&=(Color& Color);
        Color& operator^=(Color& Color);

        static Color Red(Uint8 shade=255);
        static Color Green(Uint8 shade=255);
        static Color Blue(Uint8 shade=255);

        static Color Yellow(Uint8 shade=255);
        static Color Cyan(Uint8 shade=255);
        static Color Magenta(Uint8 shade=255);

        static Color DarkRed(Uint8 shade=128);
        static Color DarkGreen(Uint8 shade=128);
        static Color DarkBlue(Uint8 shade=128);
        static Color DarkYellow(Uint8 shade=128);
        static Color DarkCyan(Uint8 shade=128);
        static Color DarkMagenta(Uint8 shade=128);

        static Color LightRed(Uint8 gray=128,Uint8 shade=255);
        static Color LightGreen(Uint8 gray=128,Uint8 shade=255);
        static Color LightBlue(Uint8 gray=128,Uint8 shade=255);
        static Color LightYellow(Uint8 gray=128,Uint8 shade=255);
        static Color LightCyan(Uint8 gray=128,Uint8 shade=255);
        static Color LightMagenta(Uint8 gray=128,Uint8 shade=255);

        static Color White(Uint8 shade=255);
        static Color LightGray(Uint8 shade=192);
        static Color DarkGray(Uint8 shade=128);
        static Color Black(Uint8 shade=0);
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
