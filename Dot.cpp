#include "Dot.hpp"

bool checkCollision(SDL_Rect a, SDL_Rect b){
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // calculate the sides of rect A
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    // if any of the sides from A are outside of B
    if (bottomA <= topB){
        return false;
    }
    if (topA >= bottomB){
        return false;
    }
    if (rightA <= leftB){
        return false;
    }
    if(leftA >= rightB){
        return false;
    }

    // if none of the sides from A are outside B
    return true;
}

Dot::Dot() {
    // init the offsets
    mPosX = 0;
    mPosY = 0;

    // init the velocities
    mVelX = 0;
    mVelY = 0;

    // set collision box dimension
    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;
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

void Dot::move(SDL_Rect& wall) {
    // move the dot left/right
    mPosX += mVelX;
    mCollider.x = mPosX;

    // if the dot went too far left/right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall)) {
        // move back
        mPosX -= mVelX;
        mCollider.x = mPosX;
    }

    // move the dot up/down
    mPosY += mVelY;
    mCollider.y = mPosY;

    // if the dot went too far up/down
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall)) {
        // move back
        mPosY -= mVelY;
        mCollider.y = mPosY;
    }
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture) {
    // show the dot
    texture->render(renderer, mPosX, mPosY);
}
