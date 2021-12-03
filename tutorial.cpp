/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <cmath>
#include <sstream>
#include <string>

#include "Constants.hpp"
#include "lTexture.hpp"
#include "lWindow.hpp"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

// our custom window
LWindow gWindow;

// display data
int gTotalDisplays = 0;
SDL_Rect* gDisplayBound = NULL;

// scene textures
LTexture gSceneTexture;


bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!\n");
        }

        // create custom window
        if (!gWindow.init(SCREEN_WIDTH, SCREEN_HEIGHT)){
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;


    return success;
}

void close() {
    gSceneTexture.free();

    //Destroy window
    gWindow.free();

    //Quit SDL subsystems
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

            //While application is running (main loop)
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } 
                    // handle windows events
                    gWindow.handleEvent(e);
                }

                // update window
                gWindow.render();
            }
        }

        //Free resources and close SDL
        close();

        return 0;
    }
}