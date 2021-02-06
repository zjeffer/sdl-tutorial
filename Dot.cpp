#include "Dot.hpp"

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b){
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // go through the A boxes;
    for(int Abox = 0; Abox < a.size(); Abox++){
        // calculate the sizes of rect A
        leftA = a[Abox].x;
        rightA = a[Abox].x + a[Abox].w;
        topA = a[Abox].y;
        bottomA = a[Abox].y + a[Abox].h;

        // go through the B boxes
        for(int Bbox = 0; Bbox < b.size(); Bbox++){
            // calculate the sizes of rect B
            leftB = b[Bbox].x;
            rightB = b[Bbox].x + b[Bbox].w;
            topB = b[Bbox].y;
            bottomB = b[Bbox].y + b[Bbox].h;

            // if no sides from A are outside of B
            if (((bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)) == false){
                // a collision is detected
                return true;
            }
        }
    }
    // if neither set of collision boxes touched
    return false;
}

void Dot::shiftColliders(){
    // the row offset
    int r = 0;

    // got through the dot's collision boxes;
    for(int set = 0; set < mColliders.size(); ++set){
        // center the collsion box
        mColliders[set].x = mPosX + (DOT_WIDTH - mColliders[set].w) / 2;

        // set the collision box at its row offset
        mColliders[set].y = mPosY + r;

        // move the row offset down the height of the collision box
        r += mColliders[set].h;
    }
}

std::vector<SDL_Rect>& Dot::getColliders(){
    return mColliders;
}

Dot::Dot(int x, int y) {
    // init the offsets
    mPosX = x;
    mPosY = y;

    // init the velocities
    mVelX = 0;
    mVelY = 0;

    // create the necessary SDL_Rects
    mColliders.resize(11);

    // initialize the collision boxes' width and height
    mColliders[0].w = 6;
    mColliders[0].h = 1;

    mColliders[1].w = 10;
    mColliders[1].h = 1;

    mColliders[2].w = 14;
    mColliders[2].h = 1;
    
    mColliders[3].w = 16;
    mColliders[3].h = 2;
    
    mColliders[4].w = 18;
    mColliders[4].h = 2;
    
    mColliders[5].w = 20;
    mColliders[5].h = 2;
    
    mColliders[6].w = 18;
    mColliders[6].h = 2;
    
    mColliders[7].w = 16;
    mColliders[7].h = 2;
    
    mColliders[8].w = 14;
    mColliders[8].h = 1;
    
    mColliders[9].w = 10;
    mColliders[9].h = 1;
    
    mColliders[10].w = 6;
    mColliders[10].h = 1;
    
    // initialize colliders relative to position
    shiftColliders();
    
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

void Dot::move(std::vector<SDL_Rect>& otherColliders) {
    // move the dot left/right
    mPosX += mVelX;
    shiftColliders();

    // if the dot went too far left/right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mColliders, otherColliders)) {
        // move back
        mPosX -= mVelX;
        shiftColliders();
    }

    // move the dot up/down
    mPosY += mVelY;
    shiftColliders();

    // if the dot went too far up/down
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mColliders, otherColliders)) {
        // move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture) {
    // show the dot
    texture->render(renderer, mPosX, mPosY);
}
