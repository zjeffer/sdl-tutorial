#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, TTF_Font* font, SDL_Color textColor){
    // get rid of preexisting texture
    free();

    // render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if(textSurface == NULL){
        printf("Unable to render text surface! SDL_ttf error: %s\n", TTF_GetError());
    } else {
        // create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(mTexture == NULL){
            printf("Unable to create texture from rendered text! SDL error: %s\n", SDL_GetError());
        } else {
            // get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // get rid of old surface
        SDL_FreeSurface(textSurface);
    }

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
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
    // set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // set clip rendering dimensions
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
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
