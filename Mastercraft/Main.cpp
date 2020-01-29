//#define GLEW_STATIC
#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>    // std::sort
#include <glimac/glm.hpp>


#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/common.hpp>

#include <glimac/Player.hpp>
#include <glimac/World.hpp>
#include <glimac/Cube.hpp>
#include <glimac/SuperChunk.hpp>
#include <glimac/Chunk.hpp>
#include <glimac/Color.hpp>
#include <glimac/CubeProgram.hpp>

using namespace glimac;
using namespace std;

int main(int argc, char** argv) {
    #ifdef _WIN32
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    #endif // WIN32

    // Initialize SDL and open a window
    /*const float windowWidth = 900.f;
    const float windowHeight = 900.f;*/
    const float windowWidth = 1920.f;
    const float windowHeight = 1080.f;
    SDLWindowManager windowManager(windowWidth, windowHeight, "MasterCraft");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    if(-1 == SDL_EnableKeyRepeat(10, 10)) {
        return EXIT_FAILURE;
    }

    // blocks texture loading
    std::unique_ptr<Image> blocksAtlas = loadImage("assets/textures/BlockAtlas.png");
    if(blocksAtlas == NULL) {
        std::cerr << "Can't load blocks atlas textures" << std::endl;
        return EXIT_FAILURE;
    }
    // Terrain map loading
    std::unique_ptr<Image> terrain = loadImage("assets/maps/terrain3_bis.png");
    if(terrain == NULL) {
        std::cerr << "Can't load terrain's map" << std::endl;
        return EXIT_FAILURE;
    }
    // Height map loading
    std::unique_ptr<Image> heightMap = loadImage("assets/maps/heightMap3.png");
    if(heightMap == NULL) {
        std::cerr << "Can't load height's map" << std::endl;
        return EXIT_FAILURE;
    }
    // Night sky texture loading
    std::unique_ptr<Image> sky = loadImage("assets/textures/sky.PNG");
    if(sky == NULL) {
        std::cerr << "Can't load sky'textures map" << std::endl;
        return EXIT_FAILURE;
    }
    // Day sky texture loading
    std::unique_ptr<Image> night = loadImage("assets/textures/night.PNG");
    if(night == NULL) {
        std::cerr << "Can't load night's textures" << std::endl;
        return EXIT_FAILURE;
    }

    glm::vec4* terrainPixels = terrain->getPixels();
    glm::vec4* heightMapPixels = heightMap->getPixels();
    const int W = (int)terrain->getWidth();
    const int H = (int)terrain->getHeight();
    if(W != (int)terrain->getWidth() || H != (int)terrain->getHeight()) {
        std::cerr << "Height map and terrain map have different size" << std::endl;
        return EXIT_FAILURE;
    }
    if(W % 16 || H % 16) {
        std::cerr << "The size should be a multiple of 16" << std::endl;
        return EXIT_FAILURE;
    }

    // Charge les shaders
    FilePath applicationPath(argv[0]);
    CubeProgram cubeProgram(applicationPath);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    glEnable(GL_DEPTH_TEST);

    //-----------------------Skybox initialization-----------------------//

    // Sky vbo
    Cube cube;
    GLint n = cube.getVertexCount();
    const ShapeVertex* vertices = cube.getDataPointer();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, n * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Sky vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), 0);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex, texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //-----------------------Textures-----------------------//

    // Textures atlas
    GLuint textures;
    glGenTextures(1, &textures);
    glBindTexture(GL_TEXTURE_2D, textures); // blocks atlas
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blocksAtlas->getWidth(), blocksAtlas->getHeight(), 0, GL_RGBA, GL_FLOAT, blocksAtlas->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // skybox
    GLuint skyTextures[2];
    glGenTextures(2, skyTextures);
    glBindTexture(GL_TEXTURE_2D, skyTextures[0]); // sky
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky->getWidth(), sky->getHeight(), 0, GL_RGBA, GL_FLOAT, sky->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, skyTextures[1]); // night's sky
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, night->getWidth(), night->getHeight(), 0, GL_RGBA, GL_FLOAT, night->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    //---------------- Initializations --------------------//

    bool done = false;
    // create a 2D array from the height map image;
    unsigned char** heightMap2DArray = new unsigned char*[W];
    int k = 0;
    for(int i = 0; i < W; i++) {
        heightMap2DArray[i] = new unsigned char[H];
        for(int j = 0; j < H; j++) {
            heightMap2DArray[i][j] = (unsigned char)(heightMapPixels[k++][0] * 256.f);
        }
    }
    // create a 2D array from the height map image;
    Color** terrainMap2DArray = new Color*[W];
    k = 0;
    for(int i = 0; i < W; i++) {
        terrainMap2DArray[i] = new Color[H];
        for(int j = 0; j < H; j++) {
            terrainMap2DArray[i][j] = Color((unsigned char)(terrainPixels[k][0] * 255.),
                                            (unsigned char)(terrainPixels[k][1] * 255.),
                                            (unsigned char)(terrainPixels[k][2] * 255.));
            k++;
        }
    }

    cubeProgram.m_Program.use();
    MasterCraftCamera mc(W/2, heightMap2DArray[W/2][H/2] + 2, H/2);
    //Player player(W/2, heightMap2DArray[W/2][H/2] + 2, H/2, W, H);
    //glm::mat4 viewMatrix = player.getViewMatrix();
    glm::mat4 viewMatrix = mc.getViewMatrix();
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), windowWidth/windowHeight, 0.1f, 1000.f);
    glm::mat4 MVMatrix, NormalMatrix;

    int lastMousePosX, lastMousePosY;
    World world(heightMap2DArray, terrainMap2DArray, W/16, H/16);
    world.addDecors(heightMap2DArray, terrainMap2DArray);
    glm::vec3 pos3D = mc.getPosition();
    //glm::vec3 pos3D = player.getPosition();
    SuperChunk superChunk(pos3D.x / 16, pos3D.z / 16, pos3D.y);
    bool isNight = false;
    int xMouseClic, yMouseClic, dist;
    GLfloat depth;

    //---------------- Application loop --------------------//

    while(!done) {

        /*glm::vec3 pos3D = player.getPosition();
        player.computeDirectionVectors();
        viewMatrix = player.getViewMatrix();
        SDL_GetRelativeMouseState(&lastMousePosX, &lastMousePosY);
        player.updateYPos(world);*/
        glm::vec3 pos3D = mc.getPosition();
        mc.computeDirectionVectors();
        viewMatrix = mc.getViewMatrix();
        SDL_GetRelativeMouseState(&lastMousePosX, &lastMousePosY);
        //mc.updateYPos(world);

        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
            // Keyboard
            if(e.type == SDL_KEYDOWN) {
                if(windowManager.isKeyPressed(SDLK_ESCAPE)) {
                    done = true; // Quit
                }
                if(windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_UP)) {
                    mc.moveFront(0.6, H);
                }
                if(windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_DOWN)) {
                    mc.moveFront(-0.6, H);
                }
                if(windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_LEFT)) {
                    mc.moveLeft(0.6, W);
                }
                if(windowManager.isKeyPressed(SDLK_d) || windowManager.isKeyPressed(SDLK_RIGHT)) {
                    mc.moveLeft(-0.6, W);
                }
                // set night or day
                if(windowManager.isKeyPressed(SDLK_e)) {
                    if(isNight)
                        isNight = false;
                    else
                        isNight = true;
                }
            }
            //Mouse
            if(e.type == SDL_MOUSEMOTION) { // Mouse motion
                SDL_GetRelativeMouseState(&lastMousePosX, &lastMousePosY);
                mc.rotateLeft(-lastMousePosX);
                mc.rotateUp(-lastMousePosY);
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                // destroy bloc
                mc.computeDirectionVectors();
                viewMatrix = mc.getViewMatrix();

                SDL_GetMouseState(&xMouseClic, &yMouseClic);
                glReadPixels(xMouseClic, windowHeight - yMouseClic - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

                glm::vec4 viewport = glm::vec4(0, 0, windowWidth, windowHeight);
                glm::vec3 wincoord = glm::vec3(xMouseClic, windowHeight - yMouseClic - 1, depth);
                glm::vec3 objcoord = glm::unProject(wincoord, viewMatrix, ProjMatrix, viewport);

                dist = glm::distance(pos3D, objcoord);
                cout << "selected bloc distance : " << dist << endl;
                printf("Coordinates in object space: %f, %f, %f\n", objcoord.x, objcoord.y, objcoord.z);
                printf("Coordinates in object space: %d, %d, %d\n",(int)objcoord.x, (int)objcoord.y, (int)objcoord.z);
                if(dist <= 6.) { // player's reach
                    world.destroyBlock((int)(objcoord.x), (int)objcoord.z, (int)objcoord.y); // adjust objcoord coordinates to select the correct block
                }
            }
        }

        //---------------- Events --------------------//
        /*SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
            // Keyboard
            if(e.type == SDL_KEYDOWN) {
                if(windowManager.isKeyPressed(SDLK_ESCAPE)) {
                    done = true; // Quit
                }
                if(windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_UP)) {
                    player.walk(world, 0.2, 0.);
                }
                else if(windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_DOWN)) {
                    player.walk(world, -0.2, 0.);
                }
                if(windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_LEFT)) {
                    player.walk(world, 0., 0.2);
                }
                else if(windowManager.isKeyPressed(SDLK_d) || windowManager.isKeyPressed(SDLK_RIGHT)) {
                    player.walk(world, 0., -0.2);
                }
                // Jump
                if(windowManager.isKeyPressed(SDLK_SPACE)) {
                    player.jump();
                }
                // set night or day
                if(windowManager.isKeyPressed(SDLK_e)) {
                    if(isNight)
                        isNight = false;
                    else
                        isNight = true;
                }
            }
            //Mouse
            if(e.type == SDL_MOUSEMOTION) { // Mouse motion
                SDL_GetRelativeMouseState(&lastMousePosX, &lastMousePosY);
                player.rotate(-lastMousePosX / 2., -lastMousePosY / 2.);
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                // destroy bloc
                player.computeDirectionVectors();
                viewMatrix = player.getViewMatrix();
                SDL_GetMouseState(&xMouseClic, &yMouseClic);
                glReadPixels(xMouseClic, windowHeight - yMouseClic - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

                glm::vec4 viewport = glm::vec4(0, 0, windowWidth, windowHeight);
                glm::vec3 wincoord = glm::vec3(xMouseClic, windowHeight - yMouseClic - 1, depth);
                glm::vec3 objcoord = glm::unProject(wincoord, viewMatrix, ProjMatrix, viewport);

                dist = glm::distance(pos3D, objcoord);
                //cout << "selected bloc distance : " << dist << endl;
                cout << "depth : " << depth << endl;
                printf("Coordinates in object space: %f, %f, %f\n", objcoord.x, objcoord.y, objcoord.z);
                printf("Coordinates in object space: %d, %d, %d\n",(int)objcoord.x, (int)objcoord.y, (int)objcoord.z);
                if(dist <= 6.) { // player's reach
                    if(e.button.button == SDL_BUTTON_LEFT) {
                        world.destroyBlock((int)(objcoord.x), (int)objcoord.z, (int)objcoord.y); // adjust objcoord coordinates to select the correct block
                        // TO FIX
                        cout << "DESTROYED!" << endl;
                    }
                    if(e.button.button == SDL_BUTTON_RIGHT) {
                        // TO DO
                        cout << "CREATE!" << endl;
                    }
                }
            }
        }*/

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glBindVertexArray(vao);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //---------------- skybox and lightning TO OPTIMIZE --------------------//

        //Set light (sun)
        //glm::vec4 lightMMatrix =  glm::rotate(glm::mat4(1.), 1.7f, glm::vec3(1, 1, 1)) * glm::vec4(128, 80, 128, 0);
        glm::vec4 lightMMatrix =  glm::rotate(glm::mat4(1), windowManager.getTime() / 5, glm::vec3(0, 0, 1)) * glm::vec4(16 * 8 * 4, 128, 16 * 8 * 4, 0);
        //glm::vec4 lightMMatrix =  glm::rotate(glm::mat4(1), windowManager.getTime() / 10, glm::vec3(0, 0, 1)) * glm::vec4(128, 8000, -128, 0);
        //glm::vec4 lightMVMatrix = viewMatrix * lightMMatrix;
        glm::vec4 lightMVMatrix = viewMatrix * lightMMatrix;

        // draw the skybox and init light coeffs according the time
        MVMatrix = glm::scale(viewMatrix, glm::vec3((float)W, 256, float(H)));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(cubeProgram.uNormalMatrix, 1, GL_FALSE , glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(cubeProgram.uMVMatrix, 1, GL_FALSE , glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(cubeProgram.uMVPMatrix, 1, GL_FALSE , glm::value_ptr(ProjMatrix * MVMatrix));
        glActiveTexture(GL_TEXTURE0);

        if (lightMMatrix.y < 0){
            isNight = true;
        }
        if (lightMMatrix.y > 0){
            isNight = false;
        }
        if(!isNight) {
            glUniform3fv(cubeProgram.uTextureKd, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
            glUniform3fv(cubeProgram.uTextureKs, 1, glm::value_ptr(glm::vec3(0., 0., 0.)));
            glUniform1f(cubeProgram.uTextureShininess, 64.);
            glUniform3fv(cubeProgram.uLightDir_vs, 1, glm::value_ptr(lightMVMatrix));
            glUniform3fv(cubeProgram.uLightIntensity, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
            glUniform3fv(cubeProgram.uLightAmbient, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
            glBindTexture(GL_TEXTURE_2D, skyTextures[0]);
        }
        else {
            glUniform3fv(cubeProgram.uTextureKd, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
            glUniform3fv(cubeProgram.uTextureKs, 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
            glUniform1f(cubeProgram.uTextureShininess, 32.);
            glUniform3fv(cubeProgram.uLightDir_vs, 1, glm::value_ptr(lightMVMatrix));
            glUniform3fv(cubeProgram.uLightIntensity, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
            glUniform3fv(cubeProgram.uLightAmbient, 1, glm::value_ptr(glm::vec3(0.15, 0.15, 0.2)));
            glBindTexture(GL_TEXTURE_2D, skyTextures[1]);
        }
        glUniform1i(cubeProgram.uTexture, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 12, n);

        //---------------- world rendering --------------------//
        superChunk.updatePos(pos3D);
        superChunk.render(world, cubeProgram, ProjMatrix, viewMatrix, textures);



        glBindVertexArray(0);
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, skyTextures);

    return EXIT_SUCCESS;
}
