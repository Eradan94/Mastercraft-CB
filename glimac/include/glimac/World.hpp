#pragma once


#include "common.hpp"
#include "glimac/Chunk.hpp"
#include "glimac/CubeProgram.hpp"
#include "glimac/DecorTypes.hpp"
#include <glimac/glm.hpp>

namespace glimac {

    class World {
        public:
            World(unsigned char** worldHeightArray, Color** worldTerrainArray, const int& chunkNumberX, const int& chunkNumberY) {
                this->chunkNumberX = chunkNumberX;
                this->chunkNumberY = chunkNumberY;
                build(worldHeightArray, worldTerrainArray, chunkNumberX, chunkNumberY);
            }
            void build(unsigned char** worldHeightArray, Color** worldTerrainArray, const int& chunkNumberX, const int& chunkNumberY);
            void renderChunk(CubeProgram& cubeProgram, glm::mat4& ProjMatrix, glm::mat4& viewMatrix, GLuint& textures, int x, int y, int z);
            void destroyBlock(const int& x, const int& y, const int& z);
            Chunk* getChunk(const int& x, const int& y, const int& z);
            bool checkBlock(glm::vec3 position);
            void addDecors(unsigned char** worldHeightArray, Color** worldTerrainArray);
            void createTree(const int& i, const int& j, const int& k);

            friend std::ostream& operator<<(std::ostream& os, const World& w);

        private:
            int chunkNumberX; // Nombre de chunks en largeur
            int chunkNumberY; // Nombre de chunks en longueur
            Chunk**** chunks;
    };

}
