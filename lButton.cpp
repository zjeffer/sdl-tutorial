#include "lButton.hpp"


// init
LButton::LButton(){
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

// sets top left position
void LButton::setPosition(int x, int y){
    mPosition.x = x;
    mPosition.y = y;
}

// handles mouse event
void LButton::handleEvents(SDL_Event* e){
    // if mouse event happens
    if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP){
        // get mouse pos
        int x,y;
        SDL_GetMouseState(&x, &y);

        // check if mouse is in button
        bool inside = true;

        // mouse is left of || right of || above || below the button
        if(x < mPosition.x || x > mPosition.x + BUTTON_WIDTH || y < mPosition.y || y  >mPosition.y + BUTTON_HEIGHT){
            inside = false;
        }

        if(!inside){
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        } else {
            switch(e->type){
                case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_BUTTON;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
            }
        }
    }
}

// shows button sprite
void LButton::render(SDL_Renderer* renderer, LTexture* texture, SDL_Rect spriteClips[]){
    // show current button sprite
    texture->render(renderer, mPosition.x, mPosition.y, &spriteClips[mCurrentSprite]);
}