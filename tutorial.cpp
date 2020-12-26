/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <string>
#include "lTexture.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// front texture
LTexture gModulatedTexture;

// back texture
LTexture gBackgroundTexture;

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // create renderer for windows
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // initialize png loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load front alpha texture
    if (!gModulatedTexture.loadFromFile(gRenderer, "img/fadeout.png")) {
        printf("Failed to load front image!\n");
        success = false;
    }else {
        gModulatedTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    // load background texture
    if(!gBackgroundTexture.loadFromFile(gRenderer, "img/fadein.png")){
        printf("Failed to load back image!\n");
        success = false;
    }


    return success;
}

void close() {
    //Free loaded image
    gModulatedTexture.free();

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            Uint8 a = 255;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if(e.type = SDL_KEYDOWN){
                        //increase alpha with w
                        if(e.key.keysym.sym == SDLK_w){
                            // cap if over 255
                            if(a+32 > 255){
                                a = 255;
                            } else {
                                a += 32;
                            }
                        } else if(e.key.keysym.sym = SDLK_s){
                            // cap if below 0
                            if(a-32 < 0){
                                a = 0;
                            } else {
                                a -= 32;
                            }
                        }
                    }
                }
                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(gRenderer);

                //render background
                gBackgroundTexture.render(gRenderer, 0, 0);

                // render front blended
                gModulatedTexture.setAlpha(a);
                gModulatedTexture.render(gRenderer, 0, 0);

                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}