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
#include "Circle.hpp"
#include "Dot.hpp"
#include "lTexture.hpp"
#include "lTimer.hpp"

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

// dot texture
LTexture gDotTexture;

// background texture
LTexture gBGTexture;

// input text texture
LTexture gInputTextTexture;

// prompt text texture
LTexture gPromptTextTexture;

// font
TTF_Font* gFont = NULL;

// data points
const int TOTAL_DATA = 10;
Sint32 gData[TOTAL_DATA];

// data points textures
LTexture gDataTextures[TOTAL_DATA];

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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED || SDL_HINT_RENDER_VSYNC);
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
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

    SDL_Color textColor = {0, 0, 0, 0xFF};
    SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

    if (!gDotTexture.loadFromFile(gRenderer, "img/dot.bmp")) {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    if(!gBGTexture.loadFromFile(gRenderer, "img/bg.png")){
        printf("Failed to load background texture\n");
        success = false;
    }

    // font
    gFont = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 28);
    if (!gFont) {
        printf("Failed to load font! %s\n", TTF_GetError());
        success = false;
    } else {
        if(!gPromptTextTexture.loadFromRenderedText(gRenderer, "Enter Data: ", gFont, {0, 0, 0})){
            printf("Failed to render prompt text texture!\n");
            success = false;
        }
    }

    // open file for reading in binary
    SDL_RWops* file = SDL_RWFromFile("nums.bin", "rb");
    if (file == NULL){
        printf("Warning: Unable to open file! SDL_Error: %s\n", SDL_GetError());
        // create file for writing
        file = SDL_RWFromFile("nums.bin", "wb");
        if (file != NULL){
            // new file created
            printf("New file created\n");
            for (int i = 0; i < TOTAL_DATA; ++i){
                gData[i] = 0;
                SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
            }
            // close file handler
            SDL_RWclose(file);
        }else {
            printf("Warning: Unable to create file! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
    } else {
        // file exists, load data
        printf("Reading file...\n");
        for (int i = 0; i < TOTAL_DATA; ++i){
            SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
        }
        // close file handler
        SDL_RWclose(file);
    }

    // initialize the data textures
    gDataTextures[0].loadFromRenderedText(gRenderer, std::to_string(gData[0]), gFont, highlightColor);
    for (int i = 1; i < TOTAL_DATA; ++i){
        gDataTextures[i].loadFromRenderedText(gRenderer, std::to_string(gData[i]), gFont, textColor);
    }

    return success;
}

void close() {
    // open data for writing
    SDL_RWops* file = SDL_RWFromFile("nums.bin", "wb");
    if (file != NULL){
        // save data
        for (int i = 0; i < TOTAL_DATA; ++i){
            SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
        }

        // close file handler
        SDL_RWclose(file);
    } else {
        printf("Unable to save file! %s\n", SDL_GetError());
    }

    //Free loaded images
    gDotTexture.free();
    gBGTexture.free();
    gPromptTextTexture.free();
    gInputTextTexture.free();
    for (int i = 0; i < TOTAL_DATA; ++i){
        gDataTextures[i].free();
    }

    // disable text input
    SDL_StopTextInput();
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

            // text color
            SDL_Color textColor = {0, 0, 0, 0xFF};
            SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

            // current data point
            int currentData = 0;

            //While application is running (main loop)
            while (!quit) {
                // the rerender text flag
                bool renderText = false;

                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    // special key input
                    else if (e.type == SDL_KEYDOWN) {
                        switch(e.key.keysym.sym){
                            // previous data entry
                            case SDLK_UP:
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, textColor);
                                --currentData;
                                if (currentData < 0){
                                    currentData = TOTAL_DATA - 1;
                                }
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, highlightColor);
                                break;
                            
                            // next data entry
                            case SDLK_DOWN:
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, textColor);
                                ++currentData;
                                if (currentData >= TOTAL_DATA){
                                    currentData = 0;
                                }
                                // rerender current entry input point
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, highlightColor);
                                break;

                            case SDLK_LEFT:
                                // if holding shift, decrement by 10
                                if (e.key.keysym.mod & KMOD_SHIFT){
                                    gData[currentData] -= 10;
                                } else {
                                    --gData[currentData];
                                }
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, highlightColor);
                                break;
                            
                            case SDLK_RIGHT:
                                // if holding shift, increment by 10
                                if (e.key.keysym.mod & KMOD_SHIFT){
                                    gData[currentData] += 10;
                                } else {
                                    ++gData[currentData];
                                }
                                gDataTextures[currentData].loadFromRenderedText(gRenderer, std::to_string(gData[currentData]), gFont, highlightColor);
                                break;
                        }
                    }
                }

                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);


                // render text textures
                gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
                for (int i = 0; i < TOTAL_DATA; ++i){
                    gDataTextures[i].render(gRenderer, (SCREEN_WIDTH - gDataTextures[i].getWidth()) / 2, gPromptTextTexture.getHeight() + (gDataTextures[i].getHeight() * i));
                }


                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}