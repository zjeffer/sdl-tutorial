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

//Key press surfaces constants
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface* gCurrentSurface = NULL;

// function to toggle fullscreen
void toggleFullscreen(SDL_Window* window) {
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : FullscreenFlag);
    SDL_ShowCursor(isFullscreen);
}

SDL_Surface* loadSurface(std::string path) {
	// the final optimized image:
	SDL_Surface* optimizedSurface = NULL;

	// load image at specified path:
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
    } else {
		// convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if(optimizedSurface == NULL){
        	printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}
		// get rid of the loaded surface
		SDL_FreeSurface(loadedSurface);
	}
    return optimizedSurface;
}

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

    // load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("img/press.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("img/up.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("img/down.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("img/left.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("img/right.bmp");

    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image! \n");
        success = false;
    }
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load default image! \n");
        success = false;
    }
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load default image! \n");
        success = false;
    }
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load default image! \n");
        success = false;
    }
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load default image! \n");
        success = false;
    }
    return success;
}

void closeWindow() {
    // deallocate surfaces
    for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
        SDL_FreeSurface(gKeyPressSurfaces[i]);
        gKeyPressSurfaces[i] = NULL;
    }

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

            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            // while application is running
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    //printf("%i\n", e.type);
                    // user requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                        printf("Event received: %i, quitting...\n", e.type);
                    }  // user presses a key
                    else if (e.type == SDL_KEYDOWN) {
						printf("%i\n", e.key.keysym.sym);
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                        }
                    }
                    // apply the image, stretched and optmized
					SDL_Rect stretchRect;
					stretchRect.x = 0;
					stretchRect.y = 0;
					stretchRect.w = SCREEN_WIDTH;
					stretchRect.h = SCREEN_HEIGHT;

                    SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
                    // update the surface
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }
    // free resources and close sdl
    closeWindow();

    return 0;
}