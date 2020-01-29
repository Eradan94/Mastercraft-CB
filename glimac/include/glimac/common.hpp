#pragma once

#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    ShapeVertex() {
	}
	ShapeVertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : position(pos), normal(nor), texCoords(tex) {
	}
};

struct ShapeVertexInstancing {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::mat4 instanceMatrix;

    ShapeVertexInstancing() {
	}
	ShapeVertexInstancing(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : position(pos), normal(nor), texCoords(tex) {
	}
};

}
