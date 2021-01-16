/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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

//Scene texture
LTexture gPromptTexture;

// The music that will be played
Mix_Music* gMusic = NULL;

// The sound effects that will be used
Mix_Chunk* gScratch = NULL;
Mix_Chunk* gHigh= NULL;
Mix_Chunk* gMedium = NULL;
Mix_Chunk* gLow = NULL;


bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!\n" );
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

                // initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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

    // load prompt texture
    if(!gPromptTexture.loadFromFile(gRenderer, "img/prompt.png")){
        printf("Failed to load prompt texture! \n");
        success = false;
    }

    // load music
    gMusic = Mix_LoadMUS("sound/beat.wav");
    if(gMusic == NULL){
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // load sounds
    gScratch = Mix_LoadWAV("sound/scratch.wav");
    if(gScratch == NULL){
        printf("Failed to load scratch! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gHigh = Mix_LoadWAV("sound/high.wav");
    if(gHigh == NULL){
        printf("Failed to load high.wav! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gMedium = Mix_LoadWAV("sound/medium.wav");
    if(gMedium == NULL){
        printf("Failed to load medium.wav! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gLow = Mix_LoadWAV("sound/low.wav");
    if(gLow == NULL){
        printf("Failed to load low.wav! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

void close() {
    //Free loaded image
    gPromptTexture.free();

    // free sound effects
    Mix_FreeChunk(gScratch);
    Mix_FreeChunk(gHigh);
    Mix_FreeChunk(gMedium);
    Mix_FreeChunk(gLow);
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;

    // free music
    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    //Quit SDL subsystems
    Mix_Quit();
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

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN){
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_1:
                            // play high sound effect
                            Mix_PlayChannel(-1, gHigh, 0);
                            break;
                        case SDLK_2:
                            // play medium sound effect
                            Mix_PlayChannel(-1, gMedium, 0);
                            break;
                        case SDLK_3:
                            // play low sound effect
                            Mix_PlayChannel(-1, gLow, 0);
                            break;
                        case SDLK_4:
                            // play scratch sound effect
                            Mix_PlayChannel(-1, gScratch, 0);
                            break;
                        case SDLK_9:
                            // if there is no music playing
                            if(Mix_PlayingMusic() == 0){
                                // play the music
                                Mix_PlayMusic(gMusic, -1);
                            }
                            else {
                                // if the music is paused
                                if(Mix_PausedMusic() == 1){
                                    Mix_ResumeMusic();
                                } else {
                                    Mix_PauseMusic();
                                }
                            }
                            break;
                        case SDLK_0:
                            // stop the music
                            Mix_HaltMusic();
                            break;
                        default:
                            break;
                        }
                    }
                }
                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                SDL_RenderClear(gRenderer);

                gPromptTexture.render(gRenderer, 0, 0);

                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}