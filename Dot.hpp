#include <SDL2/SDL.h>

#include "Constants.hpp"
#include "lTexture.hpp"

// the dot that will move around on the screen
class Dot {
   public:
    // the dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    // maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    // initialize the variables
    Dot();

    // takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    // moves the dot
    void move();

    // shows the dot on the screen
    void render(SDL_Renderer* renderer, LTexture* texture);

   private:
    // the x and y offsets of the dot
    int mPosX, mPosY;

    // the velocity of the dot
    int mVelX, mVelY;
};