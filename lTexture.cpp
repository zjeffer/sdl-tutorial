#include <SDL2/SDL_image.h>

#include <string>
#include "./lTexture.hpp"


LTexture::LTexture() {
    // initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

// deallocates memory
LTexture::~LTexture() {
    // deallocate
    free();
}

// loads image at specified path
bool LTexture::loadFromFile(SDL_Renderer* renderer, std::string path) {
    // get rid of preexisting texture
    free();

    // the final texture
    SDL_Texture* newTexture = NULL;

    // load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        // color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));
        // create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            // get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    // return success
    mTexture = newTexture;
    return mTexture != NULL;
}

// deallocates texture
void LTexture::free() {
    // free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    // modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    // set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
    // modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

// renders texture at given point
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
    // set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // set clip rendering dimensions
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

// gets image dimensions
int LTexture::getWidth() {
    return mWidth;
}
int LTexture::getHeight() {
    return mHeight;
}


// the actual hardware texture
SDL_Texture* mTexture;

// image dimensions
int mWidth;
int mHeight;
