#include "lDot.hpp"

#include "lParticle.hpp"

Dot::Dot(LTexture* dotTexture, LTexture* particleTextures[TOTAL_PARTICLES]) {
	// init the offsets
	mPosX = 0;
	mPosY = 0;

	// init the velocities
	mVelX = 0;
	mVelY = 0;

	// set the dot texture
	mDotTexture = dotTexture;

	// update mParticleTextures with the passed in textures
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		mParticleTextures[i] = particleTextures[i];
	}

	// initialize particles
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		particles[i] = new Particle(mPosX, mPosY, mParticleTextures);
	}
}

Dot::~Dot() {
	// delete particles
	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		delete particles[i];
	}
}

int Dot::getPosX() {
	return mPosX;
}

int Dot::getPosY() {
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

void Dot::renderParticles(SDL_Renderer* renderer) {
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		// delete and replace dead particles
		if (particles[i]->isDead()) {
			delete particles[i];
			particles[i] = new Particle(mPosX, mPosY, mParticleTextures);
		}
	}

	// show particles
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		particles[i]->render(renderer);
	}
}

void Dot::move() {
	// move the dot left/right
	mPosX += mVelX;

	// if the dot went too far left/right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > LEVEL_WIDTH)) {
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

void Dot::render(SDL_Renderer* renderer) {
	// show the dot
	mDotTexture->render(renderer, mPosX, mPosY);

	renderParticles(renderer);
}
