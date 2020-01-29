#include "glimac/Color.hpp"
#include <iostream>

namespace glimac {

    bool Color::operator==(const Color& other) const {
        return (other.r == r && other.g == g && other.b == b);
    }

    Color Color::operator+(const Color& other) const {
        return Color(other.r + r, other.g + g, other.b + b);
    }

    Color Color::operator-(const Color& other) const {
        return Color(r - other.r, g - other.g, b - other.b);
    }

    /* n is the number of decors
    */
    bool Color::similarColor(unsigned char n) const {
        return (r >= 0 && r <= n);
    }

    std::ostream& operator<<(std::ostream& os, const Color& c) {
        os << (int)c.r << " " << (int)c.g << " " << (int)c.b << std::endl;
        return os;
    }
}
