#pragma once

#include "common.hpp"

#include <glimac/Color.hpp>
#include "glimac/BlockTypes.hpp"
#include "glimac/DecorTypes.hpp" // UNUSED
#include "glimac/TexturesAtlas.hpp"
#include "glimac/common.hpp"
#include <glimac/glm.hpp>

#include <vector>

namespace glimac {

    //A chunk is 16 * 16  * 16 blocks

    #define VERTEX_ATTR_POSITION 0
    #define VERTEX_ATTR_NORMAL 1
    #define VERTEX_ATTR_TEXTURE 2

    #define SIZE_CHUNK_X 16
    #define SIZE_CHUNK_Y 16
    #define SIZE_CHUNK_Z 16

    class Chunk {
    public:
        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        Chunk(unsigned char** wordlHeightArray, Color** worldTerrainArray, const int& x, const int& y, const int& z) {
            build(wordlHeightArray, worldTerrainArray, x, y, z); // Construction (voir le .cpp)
            glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao);
            changed = true;
            size = 0;
        }
        Chunk() {
            blocks = new int**[SIZE_CHUNK_X];
            for(int i = 0; i < SIZE_CHUNK_X; i++) {
                blocks[i] = new int*[SIZE_CHUNK_Y];
                for(int j = 0; j < SIZE_CHUNK_Y; j++) {
                    blocks[i][j] = new int[SIZE_CHUNK_Z];
                    for(int k = 0; k < SIZE_CHUNK_Z; k++) {
                        blocks[i][j][k] = -1;
                    }
                }
            }
            glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao);
            changed = true;
            size = 0;
        }
        ~Chunk();
        void build(unsigned char** wordlHeightArray, Color** worldTerrainArray, const int& x, const int& y, const int& z);
        int getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, int type);
        void updateChunk();
        void renderChunk();
        void destroyBlock(int x, int y, int z);
        bool checkBlock(glm::vec3 position);

        friend std::ostream& operator<<(std::ostream& os, const Chunk& c);

    private:
        int*** blocks;
        bool changed;
        GLuint vbo;
        GLuint vao;
        int size; // Nombre de sommets
    };

}
