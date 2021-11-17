#include <stdio.h>
#include <sstream>
#include "lWindow.hpp"

LWindow::LWindow(){
    // initialize non-existent window
    mWindow = NULL;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullscreen = false;
    mMinimized = false;
    mWidth = 0;
    mHeight = 0;
}

bool LWindow::init(int width, int height){
    // create window
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (mWindow != NULL){
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = width;
        mHeight = height;

        // create renderer for window
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mRenderer == NULL){
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            mWindow = NULL;
        }
        else{
            // initialize renderer color
            SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // grab window id
            mWindowID = SDL_GetWindowID(mWindow);
            
            // flag as opened
            mShown = true;
        }
    } else {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }
    return mWindow != NULL && mRenderer != NULL;
}

SDL_Renderer* LWindow::createRenderer(){
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return mRenderer;
}


void LWindow::handleEvent(SDL_Event& e){
    // window event occured
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID){
        // caption update flag
        bool updateCaption = false;
        switch (e.window.event){
            // window appeared
            case SDL_WINDOWEVENT_SHOWN:
                mShown = true;
                break;

            // window disappeared
            case SDL_WINDOWEVENT_HIDDEN:
                mShown = false;
                break;
            
            // get new dimensions and repaint on window size change
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                mWidth = e.window.data1;
                mHeight = e.window.data2;
                SDL_RenderPresent(mRenderer);
                break;

            // repaint on exposure
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(mRenderer);
                break;

            // mouse entered window
            case SDL_WINDOWEVENT_ENTER:
                mMouseFocus = true;
                updateCaption = true;
                break;

            // mouse left window
            case SDL_WINDOWEVENT_LEAVE:
                mMouseFocus = false;
                updateCaption = true;
                break;

            // window has keyboard focus
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                mKeyboardFocus = true;
                updateCaption = true;
                break;

            // window lost keyboard focus
            case SDL_WINDOWEVENT_FOCUS_LOST:
                mKeyboardFocus = false;
                updateCaption = true;
                break;

            // window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
                mMinimized = true;
                break;

            // window maxized
            case SDL_WINDOWEVENT_MAXIMIZED:
                mMinimized = false;
                break;

            // window restored
            case SDL_WINDOWEVENT_RESTORED:
                mMinimized = false;
                break;

            // hide on close
            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(mWindow);
                break;
        }
        // update caption with new data (if necessary)
        if (updateCaption){
            std::stringstream caption;
            caption << "SDL Tutorial " << mWindowID << " - Mouse Focus: " << ((mMouseFocus) ? "On" : "Off") << " Keyboard Focus: " << ((mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(mWindow, caption.str().c_str());
            printf("Current window focus: %s\n", caption.str().c_str());
        }
    } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
        if (mFullscreen){
            SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
            mFullscreen = false;
        } else {
            SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
            mFullscreen = true;
            mMinimized = false;
        }
    }
}

void LWindow::focus(){
    // restore window if needed
    if (!mShown){
        SDL_ShowWindow(mWindow);
    }

    // raise window
    SDL_RaiseWindow(mWindow);
}

void LWindow::render(){
    if (!mMinimized){
        // clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);

        // update screen
        SDL_RenderPresent(mRenderer);
    }
}

void LWindow::free(){
    if (mWindow != NULL){
        SDL_DestroyWindow(mWindow);
    }
    mMouseFocus = false;
    mKeyboardFocus = false;
    mWidth = 0;
    mHeight = 0;
}

int LWindow::getWidth(){
    return mWidth;
}

int LWindow::getHeight(){
    return mHeight;
}

bool LWindow::hasMouseFocus(){
    return mMouseFocus;
}

bool LWindow::hasKeyboardFocus(){
    return mKeyboardFocus;
}

bool LWindow::isMinimized(){
    return mMinimized;
}

bool LWindow::isShown(){
    return mShown;
}