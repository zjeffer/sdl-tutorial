#include "Dot.hpp"

Dot::Dot() {
    // init the offsets
    mPosX = 0;
    mPosY = 0;

    // init the velocities
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e) {
    // if a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        // adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        // adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY -= DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
        }
    }
}

void Dot::move() {
    // move the dot left/right
    mPosX += mVelX;

    // if the dot went too far left/right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
        // move back
        mPosX -= mVelX;
    }

    // move the dot up/down
    mPosY += mVelY;

    // if the dot went too far up/down
    if ((mPosY < 0) || mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
        // move back
        mPosY -= mVelY;
    }
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture) {
    // show the dot
    texture->render(renderer, mPosX, mPosY);
}
