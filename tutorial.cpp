/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
// #include <SDL.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// function to toggle fullscreen
void toggleFullscreen(SDL_Window* window) {
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : FullscreenFlag);
    SDL_ShowCursor(isFullscreen);
}

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init() {
    // initialization flag
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // create window
        gWindow = SDL_CreateWindow("SDL Tutorial p2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia() {
    // loading success flag
    bool success = true;

    // load splash image
    const char* imagePath = "img/hello_world.bmp";
    gHelloWorld = SDL_LoadBMP(imagePath);
    if (gHelloWorld == NULL) {
        printf("Unable to load image %s SDL_Error: %s\n", imagePath, SDL_GetError());
        success = false;
    }
    return success;
}

void closeWindow() {
    // deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    // destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // quit sdl subsystems
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            // main loop flag
            bool quit = false;

            // event handler
            SDL_Event e;

            // while application is running
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    //printf("%i\n", SDL_PollEvent(&e));
                    // user requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                        printf("Event received: %i\n", e.type);
                    }

                    // apply the loaded surface (gHelloWorld) onto the screen surface
                    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }
    // free resources and close sdl
    closeWindow();

    return 0;
}