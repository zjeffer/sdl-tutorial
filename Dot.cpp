#include "Dot.hpp"

double distanceSquared(int x1, int y1, int x2, int y2){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}

bool checkCollision(Circle& a, Circle& b){
    // calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    // if the distance between the centers of the circle is less than the sum of their radii
    if(distanceSquared(a.x, a.y, b.x, b.y) < totalRadiusSquared){
        // the circles have collided
        return true;
    }

    // if not
    return false;
}

bool checkCollision(Circle& a, SDL_Rect& b){
    // closest point on collision box
    int cX, cY;

    // find closest x offset
    if (a.x < b.x){
        cX = b.x;
    } else if (a.x > b.x + b.w){
        cX = b.x + b.w;
    } else {
        cX = a.x;
    }

    // find closest y offset
    if (a.y < b.y){
        cY = b.y;
    } else if (a.y > b.y + b.h){
        cY = b.y + b.h;
    } else {
        cY = a.y;
    }

    // if the closest point is inside the circle
    if(distanceSquared(a.x, a.y, cX, cY) < a.r * a.r){
        // this box and the circle have collided
        return true;
    }

    return false;
}

void Dot::shiftColliders(){
    //Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}

Circle& Dot::getColliders(){
    return mCollider;
}

Dot::Dot(int x, int y) {
    // init the offsets
    mPosX = x;
    mPosY = y;

    // init the velocities
    mVelX = 0;
    mVelY = 0;

    // create the necessary SDL_Rects
    mCollider.r = DOT_WIDTH / 2;
    
    // initialize colliders relative to position
    shiftColliders();
    
}

int Dot::getPosX()
{
	return mPosX;
}

int Dot::getPosY()
{
	return mPosY;
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

void Dot::move(SDL_Rect& square, Circle& circle) {
    // move the dot left/right
    mPosX += mVelX;
    shiftColliders();

    // if the dot went too far left/right
    if ((mPosX - mCollider.r < 0) || (mPosX + mCollider.r > LEVEL_WIDTH) || checkCollision(mCollider, circle) || checkCollision(mCollider, square)) {
        // move back
        mPosX -= mVelX;
        shiftColliders();
    }

    // move the dot up/down
    mPosY += mVelY;
    shiftColliders();

    // if the dot went too far up/down
    if ((mPosY - mCollider.r < 0) || (mPosY + mCollider.r > LEVEL_HEIGHT)  || checkCollision(mCollider, circle) || checkCollision(mCollider, square)) {
        // move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Dot::move(){
    // move the dot left/right
    mPosX += mVelX;

    // if the dot went too far left/right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH> LEVEL_WIDTH)) {
        // move back
        mPosX -= mVelX;
    }

    // move the dot up/down
    mPosY += mVelY;

    // if the dot went too far up/down
    if ((mPosY < 0) || (mPosY + DOT_WIDTH > LEVEL_HEIGHT)) {
        // move back
        mPosY -= mVelY;
    }
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture, int camX, int camY) {
    // show the dot
    texture->render(renderer, mPosX - mCollider.r - camX, mPosY - mCollider.r - camY);
}
