#include <stdlib.h>
#include "lParticle.hpp"


Particle::Particle(int x, int y, LTexture* textures[TOTAL_PARTICLES]){
    // set offsets
    mPosX = x - 5 + (rand() % 25);
    mPosY = y - 5 + (rand() % 25);

    // initialize animation
    mFrame = rand() % 5;

    // set type 
    switch(rand() % 3){
        case 0: mTexture = textures[0]; break;
        case 1: mTexture = textures[1]; break;
        case 2: mTexture = textures[2]; break;
    }

    mShimmerTexture = textures[3];
}

Particle::~Particle(){
    // free texture
    mTexture = NULL;
    mShimmerTexture = NULL;
}

void Particle::render(SDL_Renderer* renderer){
    // show image
    mTexture->render(renderer, mPosX, mPosY);

    // show shimmer
    if (mFrame % 2 == 0){
        mShimmerTexture->render(renderer, mPosX, mPosY);
    }

    // animate
    mFrame++;
}

bool Particle::isDead(){
    return mFrame > 10;
}

