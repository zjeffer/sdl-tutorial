/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
// #include <SDL.h>
#include <SDL2/SDL.h>
#include <stdio.h>

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


int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			
			//Update the surface
			SDL_UpdateWindowSurface( window );

			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

			// toggle fullscreen using f
			// quit program using q
			bool running = true;
			for(SDL_Event event; running;){
				SDL_WaitEvent(&event);
				if(event.type == SDL_KEYDOWN){
					switch(event.key.keysym.sym){
						case SDLK_f: toggleFullscreen(window); break;
						case SDLK_q: running = false; break;
					}
				}
			}

			//Wait two seconds
			//SDL_Delay(2000);
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

