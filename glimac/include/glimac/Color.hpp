#pragma once

#include <iostream>
#include "glm.hpp"

namespace glimac {

    class Color {
        private:
            unsigned char r;
            unsigned char g;
            unsigned char b;

        public:
            Color(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b) {
            }
            Color() {
            }
            bool operator==(const Color& other) const;
            Color operator+(const Color& other) const;
            Color operator-(const Color& other) const;
            bool similarColor(unsigned char n) const;

            friend std::ostream& operator<<(std::ostream& os, const Color& c);
    };

}
