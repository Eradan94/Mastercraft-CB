#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "glm.hpp"

namespace glimac {

    class TexturesAtlas {
        private:

        public:
            static std::map<int, std::vector<std::pair<float, float>>> texturesCoord;
            static std::vector<std::pair<float, float>> getTexturesCoord(int type) {
				return TexturesAtlas::texturesCoord.at(type);
			}
    };

}
