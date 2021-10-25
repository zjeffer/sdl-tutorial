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
        printf("Failed to load font!\n");
        success = false;
    }

    return success;
}

void close() {
    //Free loaded images
    gDotTexture.free();
    gBGTexture.free();
    gPromptTextTexture.free();
    gInputTextTexture.free();

    // disable text input
    SDL_StopTextInput();

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

            // the dot that will be moving around the screen
            Dot dot(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

            // text color
            SDL_Color textColor = {0, 0, 0};

            // the current input text
            std::string inputText = "Some Text";
            gInputTextTexture.loadFromRenderedText(gRenderer, inputText.c_str(), gFont, textColor);

            gPromptTextTexture.loadFromRenderedText(gRenderer, "Enter Text:", gFont, textColor);

            // enable text input
            SDL_StartTextInput();

            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

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
                        // handle backspace
                        if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
                            if (SDL_GetModState() & KMOD_CTRL) {
                                inputText = "";
                            } else {
                                inputText.pop_back();
                            }
                            renderText = true;
                        }
                        // handle copy
                        else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                            SDL_SetClipboardText(inputText.c_str());
                        }
                        // handle paste
                        else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                            inputText += SDL_GetClipboardText();
                            renderText = true;
                        }
                    } else if (e.type == SDL_TEXTINPUT){
                        // not copy or pasting
                        if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
                            // append character
                            inputText += e.text.text;
                            renderText = true;
                        }
                    }
                    // handle input for the dot
                    dot.handleEvent(e);
                }

                dot.move();

                // rerender text if needed
                if (renderText){
                    // text is not empty
                    if (inputText != ""){
                        // render new text
                        gInputTextTexture.loadFromRenderedText(gRenderer, inputText.c_str(), gFont, textColor);
                    }
                    // text is empty
                    else {
                        // render space texture
                        gInputTextTexture.loadFromRenderedText(gRenderer, " ", gFont, textColor);
                    }
                }

                // clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // render background
                gBGTexture.render(gRenderer, 0, 0);

                // render text textures
                // TODO
                gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
                gInputTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gPromptTextTexture.getHeight()) / 2);

                // render objects
                dot.render(gRenderer, &gDotTexture, camera.x, camera.y);

                // update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}