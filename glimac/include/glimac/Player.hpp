#pragma once


#include "glm.hpp"
#include "MasterCraftCamera.hpp"
#include "World.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <vector>

namespace glimac {
    #define JUMP_HEIGHT 5
    #define JUMP_SPEED 0.25
    #define FALL_SPEED 0.20
    #define WALK_SPEED 0.2

    enum State {
        WALKING,
        FALLING,
        JUMPING
    };
    class Player {
        public :
            Player(int _x, int _y, int _z, int _W, int _H) : camera(_x, _y, _z), H(_H), W(_W) {
                state = WALKING;
                currentJumpHeight = 0.;
            }
            void walk(World& world, float x, float y);
            glm::mat4 getViewMatrix() const;
            glm::vec3 getPosition();
            glm::vec3 getUpVector();
            glm::vec3 getFrontVector();
            glm::vec3 getLeftVector();
            void computeDirectionVectors();
            void rotate(float degreesX, float degreesY);
            void jump();
            void updateYPos(World& world);
            void sideCollision(float& x, float& y, World& world);
            bool verticalCollision(World& world);
            int horizontalCollision(World& world, float x, float y);
        private :
            MasterCraftCamera camera;
            int H;
            int W;
            State state;
            float currentJumpHeight;
    };

}
