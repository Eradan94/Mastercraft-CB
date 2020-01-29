#pragma once

#include "common.hpp"

#include <glimac/Chunk.hpp>
#include "glimac/World.hpp"
#include "glimac/CubeProgram.hpp"
#include "glimac/common.hpp"
#include <glimac/glm.hpp>

#include <vector>

namespace glimac {

    //A super chunk is 16 * 16  * 16 chunks

    #define SIZE_CHUNK_GROUP_X 32
    #define SIZE_CHUNK_GROUP_Y 32
    #define SIZE_CHUNK_GROUP_Z 16

    class SuperChunk {
    public:
        SuperChunk(int x, int y, int z) : playerPosX(x), playerPosY(y), playerPosZ(z) {

        }

        void setPosX(int x);
        void setPosY(int y);
        void setPosZ(int z);

        int getPosX();
        int getPosY();
        int getPosZ();

        void updatePos(glm::vec3 pos3D);

        void render(World& world, CubeProgram& cubeProgramm, glm::mat4& ProjMatrix, glm::mat4& viewMatrix, GLuint& textures);

        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        /*SuperChunk(World world, int xPos, int yPos, int zPos) {
            memset(c, 0, sizeof(c));
            build(world, xPos, yPos, zPos);
        }
        ~SuperChunk();
        void build(World world, int xPos, int yPos, int zPos);*/

    private:
        //Chunk *c[SIZE_CHUNK_GROUP_X][SIZE_CHUNK_GROUP_Y][SIZE_CHUNK_GROUP_Z];
        int playerPosX;
        int playerPosY;
        int playerPosZ;
    };

}
