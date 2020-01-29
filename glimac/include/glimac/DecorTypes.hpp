#pragma once

#include "glm.hpp"
#include "Color.hpp"

#include <vector>


/* UNUSED
*/

namespace glimac {
    class DecorTypes {
        public :
            static const unsigned char n = 1;
            static Color colors[n];

            /* Return the type of the decor (his index in the array
            */
            static int getType(const Color& color) {
                for(int i = 0; i < n; i++) {
                    if(DecorTypes::colors[i] == color) {
                        return i;
                    }
                }
                return -1;
            }
        private :

    };
}
