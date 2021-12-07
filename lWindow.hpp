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

	// focuses on window
	void focus();

	// shows window contents
	void render();

	// deallocates internals
	void free();

	// Window dimensions
	int getWidth();
	int getHeight();

	// window focus
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();

	// get the renderer
	SDL_Renderer* getRenderer();

   private:
	// the renderer
	SDL_Renderer* mRenderer;

	// window data
	SDL_Window* mWindow;
	int mWindowID;
	int mWindowDisplayID;

	// window dimensions
	int mWidth;
	int mHeight;

	// window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullscreen;
	bool mMinimized;
	bool mShown;
};