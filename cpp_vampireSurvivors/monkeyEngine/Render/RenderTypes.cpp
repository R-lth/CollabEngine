#include "RenderTypes.h"

namespace Monkey
{
    const Color Color::White(1.f, 1.f, 1.f);
    const Color Color::Black(0.f, 0.f, 0.f);
    const Color Color::Red(1.f, 0.f, 0.f);
    const Color Color::Green(0.f, 1.f, 0.f);
    const Color Color::Blue(0.f, 0.f, 1.f);

    Color::Color(float red, float green, float blue) : red(red), green(green), blue(blue)
    {
    }

    Color& Monkey::Color::operator=(const D2D1_COLOR_F& other) {
        red = other.r;
        green = other.g;
        blue = other.b;
        
        return *this;
    }
}