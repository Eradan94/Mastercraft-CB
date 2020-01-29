#include <iostream>
#include "glimac/common.hpp"
#include "glimac/World.hpp"
#include <glimac/glm.hpp>

namespace glimac {
    /* ChunkNumbers are length and height of the images, but divided by 16 to get the size of the chunk array
    */
    void World::build(unsigned char** worldHeightArray, Color** worldTerrainArray, const int& chunkNumberX,  const int& chunkNumberY) {
        chunks = new Chunk***[chunkNumberX];
        for(int i = 0; i < chunkNumberX; i++) {
            chunks[i] = new Chunk**[chunkNumberY];
            for(int j = 0; j < chunkNumberY; j++) {
                chunks[i][j] = new Chunk*[16];
                for(int k = 0; k < 16; k++) {
                    chunks[i][j][k] = new Chunk(worldHeightArray, worldTerrainArray, i * 16, j * 16, k * 16);
                }
            }
        }
    }

    Chunk* World::getChunk(const int& x, const int& y, const int& z) {
        if(x < 0 || x >= chunkNumberX || y < 0 || y >= chunkNumberY || z < 0 || z >= 16) {
            return NULL;
        }
        return chunks[x][y][z];
    }

    void World::renderChunk(CubeProgram& cubeProgram, glm::mat4& ProjMatrix, glm::mat4& viewMatrix, GLuint& textures,  int x, int y, int z) {
        if(x < 0 || x >= chunkNumberX || y < 0 || y >= chunkNumberY || z < 0 || z >= 16) {
            return;
        }
        //glm::mat4 MVMatrix = glm::translate(viewMatrix, glm::vec3(x * 16, y * 16, z * 16));
        //pour inverser y et z
        glm::mat4 MVMatrix = glm::translate(viewMatrix, glm::vec3(x * 16, z * 16, y * 16));

        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(cubeProgram.uNormalMatrix, 1, GL_FALSE , glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(cubeProgram.uMVMatrix, 1, GL_FALSE , glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(cubeProgram.uMVPMatrix, 1, GL_FALSE , glm::value_ptr(ProjMatrix * MVMatrix));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures); // dirt top
        glUniform1i(cubeProgram.uTexture, 0);
        //std::cout << "rendering..." << std::endl;
        chunks[x][y][z]->renderChunk();
    }

    void World::destroyBlock(const int& x, const int& y, const int& z) {
        int xChunk, yChunk, zChunk;
        xChunk = x / chunkNumberX;
        yChunk = y / chunkNumberY;
        zChunk = z / 16;
        std::cout << "block in World coordinates : " << x << " " << y << " " << z << std::endl;
        std::cout << "Coord of chunk : " << xChunk << " " << yChunk << " " << zChunk << std::endl;
        std::cout << "block Coordinates in chunk : "<< x % SIZE_CHUNK_X << " " << y % SIZE_CHUNK_Y << " " << z % SIZE_CHUNK_Z << std::endl;
        chunks[xChunk][yChunk][zChunk]->destroyBlock(x % SIZE_CHUNK_X, y % SIZE_CHUNK_Y, z % SIZE_CHUNK_Z);
    }

    bool World::checkBlock(glm::vec3 position) {
        glm::vec3 positionInChunk((int)(position.x) % SIZE_CHUNK_X, (int)(position.y) % SIZE_CHUNK_Y, (int)(position.z) % SIZE_CHUNK_Z);
        return chunks[(int)(position.x) / chunkNumberX][(int)(position.z) / chunkNumberY][(int)(position.y) / 16]->checkBlock(positionInChunk);
    }

    void World::createTree(const int& i, const int& j, const int& k) {
        chunks[i / chunkNumberX][j / chunkNumberY][k / 16]->setBlock(i % 16, j % 16, k % 16, 5);
        chunks[i / chunkNumberX][j / chunkNumberY][(k + 1) / 16]->setBlock(i % 16, j % 16, (k + 1) % 16, 5);
        chunks[i / chunkNumberX][j / chunkNumberY][(k + 2) / 16]->setBlock(i % 16, j % 16, (k + 2) % 16, 5);
        chunks[i / chunkNumberX][j / chunkNumberY][(k + 3) / 16]->setBlock(i % 16, j % 16, (k + 3) % 16, 5);
    }

    void World::addDecors(unsigned char** worldHeightArray, Color** worldTerrainArray) {
        int terrain;
        int terrainHeight;
        int type;
        for(int i = 0; i < chunkNumberX * SIZE_CHUNK_X; i++) {
            for(int j = 0; j < chunkNumberY * SIZE_CHUNK_Y; j++) {
                terrainHeight = (int)worldHeightArray[i][j];
                terrain = BlockTypes::getType(worldTerrainArray[i][j]); // -1 if air block or decor
                if(terrain == -1) { // Maybe be there is a decor here, in this case we get the terrain (the color of the decor is : terrainColor + (r, 0, 0))
                    terrain = BlockTypes::getGroundTypeFromColor(worldTerrainArray[i][j]);
                    if(terrain != -1) {
                        //std::cout << "decor at " << i << " " << j << " " << terrainHeight << std::endl;
                        type = DecorTypes::getType(worldTerrainArray[i][j]);
                        //std::cout << type << std::endl;
                        if(type == 0) {
                            //std::cout << "Create tree!" << std::endl;
                            createTree(i, j, terrainHeight);
                        }
                    }
                }
            }
        }
    }
}
