#pragma once

#include <SDL2/SDL.h>


class LWindow {
	public:
		// initializes internals
		LWindow();

		// creates window
		bool init(int width, int height);

		// creates renderer from internal window
		SDL_Renderer* createRenderer();

		// handles window events
		void handleEvent(SDL_Event& e);

		// deallocates internals
		void free();

		// Window dimensions
		int getWidth();
		int getHeight();

		// window focus
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
	
	private:
		// the renderer
		SDL_Renderer* mRenderer;

		// window data
		SDL_Window* mWindow;

		// window dimensions
		int mWidth;
		int mHeight;

		// window focus
		bool mMouseFocus;
		bool mKeyboardFocus;
		bool mFullscreen;
		bool mMinimized;
};