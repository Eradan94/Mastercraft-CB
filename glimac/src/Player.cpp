#include "glimac/Player.hpp"
#include <iostream>

namespace glimac {
    void Player::walk(World& world, float x, float y) {
        if(state == WALKING && !(verticalCollision(world))) { // if the player fall or jump, he can't move on x and z axes using keyboard
            if(!(horizontalCollision(world, x, y))) {
                camera.moveFront(x, H);
                camera.moveLeft(y, W);
            }
        }
    }

    glm::mat4 Player::getViewMatrix() const {
        return camera.getViewMatrix();
    }

    glm::vec3 Player::getPosition() {
        return camera.getPosition();
    }

    glm::vec3 Player::getUpVector() {
        return camera.getUpVector();
    }

    glm::vec3 Player::getFrontVector() {
        return camera.getFrontVector();
    }

    glm::vec3 Player::getLeftVector() {
        return camera.getLeftVector();
    }

    void Player::computeDirectionVectors() {
        camera.computeDirectionVectors();
    }

    void Player::rotate(float degreesX, float degreesY) {
        camera.rotateLeft(degreesX);
	    camera.rotateUp(degreesY);
    }

    void Player::jump() {
        if(state == WALKING) {
            state = JUMPING;
        }
    }

    void Player::updateYPos(World& world) {
        if(state == JUMPING) {
            currentJumpHeight += JUMP_SPEED;
            camera.moveAbove(JUMP_SPEED, H); /* Corriger l'argument H*/
            if(currentJumpHeight >= JUMP_HEIGHT) {
                currentJumpHeight = 0;
                state = FALLING;
            }
        }
        if(state == FALLING) {
            if(!(verticalCollision(world))) { // the player have not hit the ground the ground so he continues to fall
                camera.moveAbove(-FALL_SPEED, H); /* Corriger l'argument H*/
            }
        }
    }

    void Player::sideCollision(float& x, float& y, World& world) {

    }

    bool Player::verticalCollision(World& world) {
        glm::vec3 position = getPosition();
        //std::cout << world.checkBlock(position) << std::endl;
        if(state == FALLING) { // OK
            position.y -= 2. + FALL_SPEED;
            if(world.checkBlock(position)) {
                // the player hit the ground
                state = WALKING;
                return true;
            }
        }
        /*else if (state == JUMPING){
            if(world.checkBlock(position)) {
                // The player hit the ceiling
                state = FALLING;
                std::cout << getPosition() << std::endl;
                return 2;
            }
        }*/
        else { // detect if the player is falling during walking
            position.y -= 3.;
            if(!(world.checkBlock(position))) {
                // The player is now falling!
                state = FALLING;
                return true;
            }
        }
        return false;
    }

    /*bool Player::horizontalCollision(World& world, float x, float y) {
        // check two blocks in the dir of walking
        glm::vec3 leftVector = getLeftVector();
        glm::vec3 position = getPosition();
        position.y -= 1;
        if(x != 0) {
            if(x < 0) { // z is pressed
                position.x -= leftVector.x * WALK_SPEED;
            }
            else { // s is pressed
                position.x += leftVector.x * WALK_SPEED;
            }
        }
        if(y != 0) { // q is pressed
            if(y < 0) {
                position.z -= leftVector.z * WALK_SPEED;
            }
            else { // d is pressed
                position.z += leftVector.z * WALK_SPEED;
            }
        }
        glm::vec3 position2 = position;
        position2.y -= 1;
        return (world.checkBlock(position) || (world.checkBlock(position2)));
    }*/

    int Player::horizontalCollision(World& world, float x, float y) {
        glm::vec3 leftVector = getLeftVector();
        glm::vec3 position = getPosition();
        glm::vec4 direction;
        glm::mat4 r(1.);
        if(x != 0 && y != 0) {
            if(x < 0 && y < 0) { // SG
                glm::rotate(r, 135.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(x > 0 && y > 0) { // ZD
                glm::rotate(r, -45.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(x < 0 && y > 0) { // ZG
                glm::rotate(r, 45.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(x > 0 && y < 0) { // SD
                glm::rotate(r, -135.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
        }
        else {
            if(x < 0) { // s
                glm::rotate(r, 180.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(x > 0) { // z
                glm::rotate(r, 0.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(y < 0) { // q
                glm::rotate(r, 90.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
            else if(y > 0) { // d
                glm::rotate(r, -90.f / 180.f * 3.1415f, glm::vec3(0, 1, 0));
            }
        }
        direction = r * glm::vec4(leftVector, 0);
        position.x += direction.x * WALK_SPEED;
        position.z += direction.z * WALK_SPEED;
        position.y -= 1;
        glm::vec3 position2 = position;
        position2.y -= 1;
        return (world.checkBlock(position) || (world.checkBlock(position2)));
    }
}
