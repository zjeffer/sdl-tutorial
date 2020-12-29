/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <string>
#include <cmath>


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

// analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// game controller 1 handler
SDL_Joystick* gGameController = NULL;

LTexture gArrowTexture;


bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!\n" );
		}

        // check for joysticks
        int numJoysticks = SDL_NumJoysticks();
        if (numJoysticks < 1){
            printf("Warning: No joysticks connected!\n");
        } else {
            printf("INFO: Number of joysticks found: %i\n", numJoysticks);
            // load joystick
            gGameController = SDL_JoystickOpen(0);
            if(gGameController == NULL){
                printf("Warning: Unable to open game controller! SDL_Error: %s\n", SDL_GetError());
            } else {
                printf("=====================\n");
                printf("Joystick name: %s\n", SDL_JoystickName(gGameController));
                printf("Number of axes: %i\n", SDL_JoystickNumAxes(gGameController));
                printf("Number of buttons: %i\n", SDL_JoystickNumButtons(gGameController));
                printf("Number of balls: %i\n", SDL_JoystickNumBalls(gGameController));
                printf("=====================\n");
            }
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // create renderer for windows
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    if (!gArrowTexture.loadFromFile(gRenderer, "img/arrow.png")){
        printf("Failed to load arrow image! SDL_image error: %s\n", SDL_GetError());
        success = false;
    }
    

    return success;
}

void close() {
    //Free loaded image
    gArrowTexture.free();

    // close game controller
    SDL_JoystickClose(gGameController);
    gGameController = NULL;

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

            double angle = 0.0;
            // normalized direction
            int xDir = 0;
            int yDir = 0;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_JOYAXISMOTION){
                        // motion on controller 0
                        if(e.jaxis.which == 0){
                            // x axis
                            if(e.jaxis.axis == 0){
                                // left of dead zone
                                if(e.jaxis.value < -JOYSTICK_DEAD_ZONE){
                                    xDir = -1;
                                }
                                // right of dead zone
                                else if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
                                    xDir = 1;
                                }else{
                                    xDir = 0;
                                }
                            }
                            // y axis
                            else if (e.jaxis.axis == 1){
                                // left of dead zone
                                if(e.jaxis.value < -JOYSTICK_DEAD_ZONE){
                                    yDir = -1;
                                }
                                // right of dead zone
                                else if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
                                    yDir = 1;
                                }else{
                                    yDir = 0;
                                }
                            }
                            
                        }
                    }
                }
                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(gRenderer);

                // calculate angle
                double joystickAngle = atan2((double) yDir, (double) xDir) * (180.0 / M_PI);
                if(xDir == 0 && yDir == 0){
                    joystickAngle = 0;
                }

                // render joystick 8 way angle
                gArrowTexture.render(gRenderer, (SCREEN_WIDTH - gArrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - gArrowTexture.getHeight()) / 2, NULL, joystickAngle);

                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}