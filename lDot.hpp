#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "Circle.hpp"
#include "Constants.hpp"
#include "lParticle.hpp"
#include "lTexture.hpp"

// the dot that will move around on the screen
class Dot {
   public:
	// the dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	// maximum axis velocity of the dot
	static const int DOT_VEL = 5;

	// initialize the variables
	Dot(LTexture* dotTexture, LTexture* particleTextures[TOTAL_PARTICLES]);

	// deallocate particles
	~Dot();

	// takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	// move the dot
	void move();

	// shows the dot on the screen
	void render(SDL_Renderer* renderer);

	// Position accessors
	int getPosX();
	int getPosY();

   private:
	// the x and y offsets of the dot
	int mPosX, mPosY;

	// the velocity of the dot
	int mVelX, mVelY;

	// the particles
	Particle* particles[TOTAL_PARTICLES];

	// show the particles
	void renderParticles(SDL_Renderer* renderer);

	LTexture* mDotTexture;
	LTexture* mParticleTextures[TOTAL_PARTICLES];
};