#include <iostream>
#include "glimac/common.hpp"
#include "glimac/SuperChunk.hpp"
#include <glimac/glm.hpp>

namespace glimac {

    void SuperChunk::setPosX(int x) {
      playerPosX = x;
    }

    void SuperChunk::setPosY(int y) {
      playerPosY = y;
    }

    void SuperChunk::setPosZ(int z) {
      playerPosZ = z;
    }

    int SuperChunk::getPosX() {
        return playerPosX;
    }
    int SuperChunk::getPosY() {
        return playerPosY;
    }
    int SuperChunk::getPosZ() {
        return playerPosZ;
    }

    void SuperChunk::updatePos(glm::vec3 pos3D) {
        if(((int)pos3D.x / 16) != playerPosX) {
            setPosX((int)pos3D.x / 16);
        }
        if(((int)pos3D.y / 16) != playerPosZ) {
            setPosZ((int)pos3D.y / 16);
        }
        if(((int)pos3D.z / 16) != playerPosY) {
            setPosY((int)pos3D.z / 16);
        }
    }

    void SuperChunk::render(World& world, CubeProgram& cubeProgramm, glm::mat4& ProjMatrix, glm::mat4& viewMatrix, GLuint& textures) {

      for(int x = playerPosX - SIZE_CHUNK_GROUP_X / 2; x < playerPosX + SIZE_CHUNK_GROUP_X / 2; x++) {
          for(int y = playerPosY - SIZE_CHUNK_GROUP_Y / 2; y < playerPosY + SIZE_CHUNK_GROUP_Y / 2; y++) {
            for(int z = playerPosZ - SIZE_CHUNK_GROUP_Z / 2; z < playerPosZ + SIZE_CHUNK_GROUP_Z / 2; z++) {
              world.renderChunk(cubeProgramm, ProjMatrix, viewMatrix, textures, x, y, z);
            }
          }
      }
    }
}
