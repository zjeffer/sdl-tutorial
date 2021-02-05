#include <SDL2/SDL.h>

#include "./lButtonSprite.hpp"
#include "./lTexture.hpp"

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

class LButton {
   public:
    // init
    LButton();

    // sets top left position
    void setPosition(int x, int y);

    // handles mouse event
    void handleEvents(SDL_Event* e);

    // shows button sprite
    void render(SDL_Renderer* renderer, LTexture* texture, SDL_Rect spriteClips[]);

   private:
    // top left pos
    SDL_Point mPosition;

    // Currently used global sprite
    LButtonSprite mCurrentSprite;
};