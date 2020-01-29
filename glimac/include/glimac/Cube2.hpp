#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

class Cube2 {
	// Alloue et construit les données (implantation dans le .cpp)
    void build();

public:
	// Constructeur: alloue le tableau de données et construit les attributs des vertex
    Cube2(): m_nVertexCount(24) {
        build(); // Construction (voir le .cpp)
    }

    // Renvoit le pointeur vers les données
    const ShapeVertexInstancing* getDataPointer() const {
        return &m_Vertices[0];
    }

    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertexInstancing> m_Vertices; // Sommets
    GLsizei m_nVertexCount; // Nombre de sommets
};

}
