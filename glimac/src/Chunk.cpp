#include <iostream>
#include <vector>
#include <utility>
#include "glimac/common.hpp"
#include "glimac/Chunk.hpp"
#include <glimac/glm.hpp>

namespace glimac {
    void Chunk::build(unsigned char** worldHeightArray, Color** worldTerrainArray, const int& x, const int& y, const int& z) {
        int terrain;
        int terrainHeight;
        blocks = new int**[SIZE_CHUNK_X];
        for(int i = 0; i < SIZE_CHUNK_X; i++) {
            blocks[i] = new int*[SIZE_CHUNK_Y];
            for(int j = 0; j < SIZE_CHUNK_Y; j++) {
                blocks[i][j] = new int[SIZE_CHUNK_Z];
                terrainHeight = (int)worldHeightArray[x + i][y + j];
                terrain = BlockTypes::getType(worldTerrainArray[x + i][y + j]); // -1 if air block or decor
                if(terrain == -1) { // Maybe be there is a decor here, in this case we get the terrain (the color of the decor is : terrainColor + (r, 0, 0))
                    terrain = BlockTypes::getGroundTypeFromColor(worldTerrainArray[x + i][y + j]);
                }
                for(int k = 0; k < SIZE_CHUNK_Z; k++) { // fill the rest with air block -1
                    blocks[i][j][k] = -1;
                }
                if(z < terrainHeight) {
                    if(terrainHeight - z >= 16) { // the chunk is not a surface chunk
                        for(int k = 0; k < SIZE_CHUNK_Z; k++) {
                            blocks[i][j][k] = terrain;
                        }
                    }
                    else {
                        for(int k = 0; k < terrainHeight - z; k++) { // surface chunk
                            blocks[i][j][k] = terrain;
                        }
                    }
                }
            }
        }
    }

    Chunk::~Chunk() {
      for(int i = 0; i < SIZE_CHUNK_X; i++) {
   			for(int j = 0; j < SIZE_CHUNK_Z; j++) {
   				delete[] blocks[i][j];
   			}
   			delete[] blocks[i];
   		}
      delete[] blocks;
      glDeleteBuffers(1, &vbo);
      glDeleteVertexArrays(1, &vao);
    }

    /* Get the type of the block (0 = air block)
  	*/
  	int Chunk::getBlock(int x, int y, int z) {
    	return blocks[x][y][z];
  	}

  	/* Set the type of block
  	*/
  	void Chunk::setBlock(int x, int y, int z, int type) {
    	blocks[x][y][z] = type;
    	changed = true;
  	}

  	void Chunk::updateChunk() {
      changed = false;
      ShapeVertex* vertices = (ShapeVertex*)malloc(sizeof(ShapeVertex) * SIZE_CHUNK_X * SIZE_CHUNK_Y * SIZE_CHUNK_Z * 36);
      int i = 0;
      int type;
      std::vector<std::pair<float, float>> texturesCoord;
      for(int x = 0; x < SIZE_CHUNK_X; x++) {
        for(int y = 0; y < SIZE_CHUNK_Y; y++) {
          for(int z = 0; z < SIZE_CHUNK_Z; z++) {
            type = blocks[x][y][z];
            //std::cout << z << std::endl;
            //std::cout << type << std::endl;

            if(type == -1) { // air block
              continue;
            }

            //get the textures Coordinates on Texture atlas
            texturesCoord = TexturesAtlas::getTexturesCoord(type);
            //std::cout << texturesCoord[0].first << std::endl;

            // Inverser y et z, renverse le cube

            // top -> front  OK
            if(y < SIZE_CHUNK_Y - 1 && blocks[x][y + 1][z] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first, texturesCoord[1].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first, texturesCoord[1].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first + OFFSET, texturesCoord[1].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first + OFFSET, texturesCoord[1].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first + OFFSET, texturesCoord[1].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[1].first, texturesCoord[1].second + OFFSET));
            }

            //front -> bottom
            if(z > 0 && blocks[x][y][z - 1] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first, texturesCoord[5].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first, texturesCoord[5].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first + OFFSET, texturesCoord[5].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first + OFFSET, texturesCoord[5].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first, texturesCoord[5].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[5].first + OFFSET, texturesCoord[5].second));
            }

            // right -> right
            if(x < SIZE_CHUNK_X - 1 && blocks[x + 1][y][z] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first, texturesCoord[2].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y + 1), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first + OFFSET, texturesCoord[2].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first, texturesCoord[2].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y + 1), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first + OFFSET, texturesCoord[2].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y + 1), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first + OFFSET, texturesCoord[2].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[2].first, texturesCoord[2].second));
            }

            // left -> left
            if(x > 0 && blocks[x - 1][y][z] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[3].first + OFFSET, texturesCoord[3].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[3].first + OFFSET, texturesCoord[3].second));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[3].first, texturesCoord[3].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y + 1), glm::vec3(1, 0., 0.), glm::vec2(texturesCoord[3].first, texturesCoord[3].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[3].first + OFFSET, texturesCoord[3].second));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y + 1), glm::vec3(1., 0., 0.), glm::vec2(texturesCoord[3].first, texturesCoord[3].second));
            }

            // back -> top
            if(z < SIZE_CHUNK_Z - 1 && blocks[x][y][z + 1] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first + OFFSET, texturesCoord[0].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first + OFFSET, texturesCoord[0].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first, texturesCoord[0].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y + 1), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first, texturesCoord[0].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first, texturesCoord[0].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y), glm::vec3(0., 1., 0.), glm::vec2(texturesCoord[0].first + OFFSET, texturesCoord[0].second + OFFSET));
            }

            // bottom -> back
            if(y > 0 && blocks[x][y - 1][z] == - 1) {
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first + OFFSET, texturesCoord[4].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z + 1, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first + OFFSET, texturesCoord[4].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first, texturesCoord[4].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z + 1, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first, texturesCoord[4].second));
                vertices[i++] = ShapeVertex(glm::vec3(x + 1, z, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first, texturesCoord[4].second + OFFSET));
                vertices[i++] = ShapeVertex(glm::vec3(x, z, y), glm::vec3(0., 0., 1.), glm::vec2(texturesCoord[4].first + OFFSET, texturesCoord[4].second + OFFSET));
            }
          }
        }
      }
      size = i;
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, size * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      free(vertices);
  	}

  	void Chunk::renderChunk() {
  	    //std::cout << "draw0 : " << changed << std::endl;
      if(changed) {
          updateChunk();
      }
      if(!size) {
        return;
      }
      //glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBindVertexArray(vao);

      glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
      glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
      glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

      glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), 0);
      glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const GLvoid*)(offsetof(ShapeVertex, normal)));
      glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
        (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
      glDrawArrays(GL_TRIANGLES, 0, size);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
  	}

  	void Chunk::destroyBlock(int x, int y, int z) {
  	    std::cout << " chunk destroy block coord : " << x << " " << y << " " << z << std::endl;
        blocks[x][y][z] = -1;
        changed = true;
  	}

  	/* Return true if the block is not a air block (-1)
  	*/
  	bool Chunk::checkBlock(glm::vec3 position) {
        return (blocks[(int)position.x][(int)position.z][(int)position.y] != -1);
  	}

    std::ostream& operator<<(std::ostream& os, const Chunk& c) {
        for(int i = 0; i < SIZE_CHUNK_X; i++) {
            for(int j = 0; j < SIZE_CHUNK_Y; j++) {
                for(int k = 0; k < SIZE_CHUNK_Z; k++) {
                    //os << (int)c.blocks[i][j][k].type << " ";
                    os << c.blocks[i][j][k];
                }
            }
            os << std::endl;
        }
        os << std::endl;
        return os;
    }
}
