#include <SDL2/SDL.h>
#include <vector>

#include "Constants.hpp"
#include "Circle.hpp"
#include "lTexture.hpp"

// the dot that will move around on the screen
class Dot {
   public:
    // the dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    // maximum axis velocity of the dot
    static const int DOT_VEL = 2;

    // initialize the variables
    Dot(int x, int y);

    // takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    // moves the dot, with collision
    void move(SDL_Rect& square, Circle& circle);

    // move the dot
    void move();

    // shows the dot on the screen
    void render(SDL_Renderer* renderer, LTexture* texture, int camX, int camY);
    
    //Position accessors
	int getPosX();
	int getPosY();

    Circle& getColliders();

   private:
    // the x and y offsets of the dot
    int mPosX, mPosY;

    // the velocity of the dot
    int mVelX, mVelY;

    // dot's collision circle
    Circle mCollider;

    // moves the collision circle relateive to the dot's offset
    void shiftColliders();
};