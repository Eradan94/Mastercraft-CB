#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cube2.hpp"
#include <glimac/glm.hpp>

namespace glimac {
	void Cube2::build() {
        //Top
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 0.), glm::vec3(0., 1., 0.), glm::vec2(1., 1.))); // 0
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 0.), glm::vec3(0., 1., 0.), glm::vec2(0., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 1.), glm::vec3(0., 1., 0.), glm::vec2(1., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 1.), glm::vec3(0., 1., 0.), glm::vec2(0., 0.)));
        //Bottom
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.), glm::vec2(1., 1.))); // 4
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 1.), glm::vec3(0., 1., 0.), glm::vec2(1., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 0.), glm::vec3(0., 1., 0.), glm::vec2(0., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 1.), glm::vec3(0., 1., 0.), glm::vec2(0., 0.)));
        //Back
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 1.), glm::vec3(0., 0., 1.), glm::vec2(0., 1.))); // 8
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 1.), glm::vec3(0., 0., 1.), glm::vec2(0., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 1.), glm::vec3(0., 0., 1.), glm::vec2(1., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 1.), glm::vec3(0., 0., 1.), glm::vec2(1., 0.)));
        //Front
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 0.), glm::vec3(0., 0., 1.), glm::vec2(1., 1.))); // 12
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 0.), glm::vec3(0., 0., 1.), glm::vec2(0., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 0.), glm::vec3(0., 0., 1.), glm::vec2(1., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 0.), glm::vec3(0., 0., 1.), glm::vec2(0., 0.)));
        //Left
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 0.), glm::vec3(1., 0., 0.), glm::vec2(1., 1.))); // 16
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 0.), glm::vec3(1., 0., 0.), glm::vec2(1., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 0., 1.), glm::vec3(1., 0., 0.), glm::vec2(0., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(1., 1., 1.), glm::vec3(1., 0., 0.), glm::vec2(0., 0.)));
        //Right
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 0.), glm::vec3(1., 0., 0.), glm::vec2(0., 1.))); // 20
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 0., 1.), glm::vec3(1., 0., 0.), glm::vec2(1., 1.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 0.), glm::vec3(1., 0., 0.), glm::vec2(0., 0.)));
        m_Vertices.push_back(ShapeVertexInstancing(glm::vec3(0., 1., 1.), glm::vec3(1., 0., 0.), glm::vec2(1., 0.)));
	}
}
