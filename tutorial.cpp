/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <cmath>
#include <sstream>
#include <string>

#include "lTexture.hpp"
#include "lTimer.hpp"

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

//Scene textures
LTexture gStartPromptTexture;
LTexture gPausePromptTexture;

// time text texture
LTexture gTimeTextTexture;

TTF_Font* gFont = NULL;


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
                
                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

    printf("start \n");


    // open the font
    gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
    
    if (gFont == NULL){
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {
        // set text color to black
        SDL_Color textColor = {0, 0, 0, 255};
		
		//Load stop prompt texture
		if( !gStartPromptTexture.loadFromRenderedText(gRenderer, "Press S to Start or Stop the Timer", gFont, textColor ) )
		{
			printf( "Unable to render start/stop prompt texture!\n" );
			success = false;
		}
		
		//Load pause prompt texture
		if( !gPausePromptTexture.loadFromRenderedText(gRenderer, "Press P to Pause or Unpause the Timer", gFont, textColor ) )
		{
			printf( "Unable to render pause/unpause prompt texture!\n" );
			success = false;
		}
    }
    
    return success;
}

void close() {
    //Free loaded images
    gPausePromptTexture.free();
    gStartPromptTexture.free();
    gTimeTextTexture.free();

    // close font
    TTF_CloseFont(gFont);
	gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    //Quit SDL subsystems
    TTF_Quit();
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

            // set text color
            SDL_Color textColor = {0,0,0,255};

			// the application timer
			LTimer timer;

            // in memory text stream
            std::stringstream timeText;

            //While application is running (main loop)
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_s: 
								if(timer.isStarted()){
									timer.stop();
								} else {
									timer.start();
								}
                                break;
							case SDLK_p:
								if(timer.isPaused()){
									timer.unpause();
								} else {
									timer.pause();
								}
								break;
                            default:
                                break;
                        }
                    }
                }
                // set text to be rendered
                timeText.str("");
                timeText << "Seconds since start time " << (timer.getTicks() / 1000.f);

                // render text
                if(!gTimeTextTexture.loadFromRenderedText(gRenderer, timeText.str().c_str(), gFont, textColor)){
                    printf( "Unable to render time texture!\n" );
                }

                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(gRenderer);

				gStartPromptTexture.render(gRenderer, (SCREEN_WIDTH - gStartPromptTexture.getWidth()) / 2, 0);
                gPausePromptTexture.render(gRenderer, (SCREEN_WIDTH - gPausePromptTexture.getWidth()) / 2, gStartPromptTexture.getHeight());
                gTimeTextTexture.render(gRenderer, (SCREEN_WIDTH - gTimeTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTimeTextTexture.getHeight())/2);

                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}