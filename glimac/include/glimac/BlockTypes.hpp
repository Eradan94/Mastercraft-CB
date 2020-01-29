#pragma once

#include "glm.hpp"
#include "Color.hpp"
#include "DecorTypes.hpp"

#include <vector>

namespace glimac {

    #define OFFSET 16. / 256.

    class BlockTypes {
        public:
            static const int n = 6;
            static Color colors[n];

            static int getType(const Color& color) {
                for(int i = 0; i < n; i++) {
                    if(BlockTypes::colors[i] == color) {
                        return i;
                    }
                }
                return -1;
            }

            /* Take a decor's color and return the type of the ground to generate the chunk
            */
            static int getGroundTypeFromColor(const Color& decorColor) {
                for(int i = 0; i < n; i++) {
                    Color diff(decorColor - colors[i]);
                    if(diff.similarColor(DecorTypes::n)) {
                        return i;
                    }
                }
                return -1;
            }
    };

}
