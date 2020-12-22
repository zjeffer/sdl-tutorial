#include <SDL2/SDL_image.h>
#include <string>

class LTexture {
    public:
        // initializes variables
        LTexture();

        // deallocates memory
        ~LTexture();

        // loads image at specified path
        bool loadFromFile(SDL_Renderer* renderer, std::string path);

        // deallocates texture
        void free();

        // set color modulation
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        // renders texture at given point
        void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL);

        // gets image dimensions
        int getWidth();
        int getHeight();

    private:
        // the actual hardware texture
        SDL_Texture* mTexture;

        // image dimensions
        int mWidth;
        int mHeight;
};