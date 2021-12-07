#pragma once

#include "lTexture.hpp"

// particle count
const int TOTAL_PARTICLES = 4;

class Particle {
   public:
	Particle(int x, int y, LTexture* textures[TOTAL_PARTICLES]);

	~Particle();

	// show the particle
	void render(SDL_Renderer* renderer);

	// check if the particle is dead
	bool isDead();

   private:
	// offsets
	int mPosX, mPosY;

	// current animation frame
	int mFrame;

	// type of particle
	LTexture* mTexture;
	LTexture* mShimmerTexture;
};