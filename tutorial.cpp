/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <cmath>
#include <sstream>
#include <string>

#include "Circle.hpp"
#include "Constants.hpp"
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

// gTextColor
SDL_Color gTextColor = {0, 0, 0, 0xff};
SDL_Color gHighlightColor = {0xFF, 0xFF, 0xFF, 0xFF};

// prompt text texture
LTexture gPromptTextTexture;

// font
TTF_Font* gFont = NULL;

// maximum number of supported recording devices
const int MAX_RECORDING_DEVICES = 10;

// maximum recording time (seconds)
const int MAX_RECORDING_SECONDS = 5;

// maximum recording time + padding
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

// the text textures that specify recording device names
LTexture gDeviceTextures[MAX_RECORDING_DEVICES];

// the various recording actions we can take
enum RecordingState {
    SELECTING_DEVICE,
    STOPPED,
    RECORDING,
    RECORDED,
    PLAYBACK,
    ERROR
};

// number of available devices
int gRecordingDeviceCount = 0;

// received audio spec
SDL_AudioSpec gReceivedRecordingSpec;
SDL_AudioSpec gReceivedPlaybackSpec;

// recording data buffer
Uint8* gRecordingBuffer = NULL;

// size of data buffer
Uint32 gBufferByteSize = 0;

// Position in data buffer
Uint32 gBufferBytePosition = 0;

// maximum position in data buffer for recording
Uint32 gBufferByteMaxPosition = 0;

// recording/playback callbacks
void audioRecordingCallback(void* userdata, Uint8* stream, int len) {
    // copy audio from stream
    memcpy(&gRecordingBuffer[gBufferBytePosition], stream, len);
    // move along buffer
    gBufferBytePosition += len;
}

void audioPlaybackCallback(void* userdata, Uint8* stream, int len) {
    // copy audio to stream
    memcpy(stream, &gRecordingBuffer[gBufferBytePosition], len);
    // move along buffer
    gBufferBytePosition += len;
}

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

    gTextColor = {0, 0, 0, 0xFF};
    gHighlightColor = {0xFF, 0, 0, 0xFF};

    // font
    gFont = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 28);
    if (!gFont) {
        printf("Failed to load font! %s\n", TTF_GetError());
        success = false;
    } else {
        // set starting prompt
        gPromptTextTexture.loadFromRenderedText(gRenderer, "Select recording device:", gFont, gTextColor);

        // get capture device count
        gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

        // no recording devices
        if (gRecordingDeviceCount < 1) {
            printf("Unable to get audio capture device! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // cap recording device count
            if (gRecordingDeviceCount > MAX_RECORDING_DEVICES) {
                gRecordingDeviceCount = MAX_RECORDING_DEVICES;
            }
            // render device names
            std::stringstream promptText;
            for (int i = 0; i < gRecordingDeviceCount; ++i) {
                // get capture device name
                promptText.str("");
                promptText << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);

                // set texture from name
                gDeviceTextures[i].loadFromRenderedText(gRenderer, promptText.str().c_str(), gFont, gTextColor);
            }
        }
    }

    return success;
}

void close() {
    gPromptTextTexture.free();
    for (int i = 0; i < MAX_RECORDING_DEVICES; ++i) {
        gDeviceTextures[i].free();
    }

    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    // free playback audio
    if (gRecordingBuffer != NULL) {
        delete[] gRecordingBuffer;
        gRecordingBuffer = NULL;
    }

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

            // set the default recording state
            RecordingState currentState = SELECTING_DEVICE;

            // audio device ids
            SDL_AudioDeviceID recordingDeviceId = 0;
            SDL_AudioDeviceID playbackDeviceId = 0;

            //While application is running (main loop)
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    switch (currentState) {
                        case SELECTING_DEVICE:
                            // on key press
                            if (e.type == SDL_KEYDOWN) {
                                // handle key press from 0 to 9
                                if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                                    // get selection index
                                    int index = e.key.keysym.sym - SDLK_0;
                                    // index is valid
                                    if (index < gRecordingDeviceCount) {
                                        // default audio spec
                                        SDL_AudioSpec desiredRecordingSpec;
                                        SDL_zero(desiredRecordingSpec);
                                        desiredRecordingSpec.freq = 44100;
                                        desiredRecordingSpec.format = AUDIO_F32;
                                        desiredRecordingSpec.channels = 2;
                                        desiredRecordingSpec.samples = 4096;
                                        desiredRecordingSpec.callback = audioRecordingCallback;

                                        // open recordig device
                                        recordingDeviceId = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, SDL_TRUE), SDL_TRUE, &desiredRecordingSpec, &gReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
                                        // if device failed to open
                                        if (recordingDeviceId == 0) {
                                            // report error
                                            printf("Failed to open audio device! SDL Error: %s\n", SDL_GetError());
                                            gPromptTextTexture.loadFromRenderedText(gRenderer, "Failed to open audio device!", gFont, gTextColor);
                                            currentState = ERROR;
                                        } else {
                                            // device opened successfully
                                            // default audio spec for playback
                                            SDL_AudioSpec desiredPlaybackSpec;
                                            SDL_zero(desiredPlaybackSpec);
                                            desiredPlaybackSpec.freq = 44100;
                                            desiredPlaybackSpec.format = AUDIO_F32;
                                            desiredPlaybackSpec.channels = 2;
                                            desiredPlaybackSpec.samples = 4096;
                                            desiredPlaybackSpec.callback = audioPlaybackCallback;

                                            // open playback device
                                            playbackDeviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desiredPlaybackSpec, &gReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

                                            // if device failed to open
                                            if (playbackDeviceId == 0) {
                                                // report error
                                                printf("Failed to open audio device! SDL Error: %s\n", SDL_GetError());
                                                gPromptTextTexture.loadFromRenderedText(gRenderer, "Failed to open audio device!", gFont, gTextColor);
                                                currentState = ERROR;
                                            } else {
                                                // device opened successfully
                                                // calculate per sample bytes
                                                int bytesPerSample = gReceivedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(gReceivedRecordingSpec.format) / 8);
                                                // calculate bytes per second
                                                int bytesPerSecond = gReceivedRecordingSpec.freq * bytesPerSample;
                                                // calculate buffer size
                                                gBufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;
                                                // calculate max buffer use
                                                gBufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;
                                                // allocate and initialize byte buffer
                                                gRecordingBuffer = new Uint8[gBufferByteSize];
                                                memset(gRecordingBuffer, 0, gBufferByteSize);

                                                // go on to next state
                                                gPromptTextTexture.loadFromRenderedText(gRenderer, "Press 1 to record for 5 seconds", gFont, gTextColor);
                                                currentState = STOPPED;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        case STOPPED:
                            // on key press
                            if (e.type == SDL_KEYDOWN) {
                                // handle key press from 0 to 9
                                if (e.key.keysym.sym == SDLK_1) {
                                    // go back to beginning of buffer
                                    gBufferBytePosition = 0;
                                    // start recording
                                    SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
                                    // go on to next state
                                    gPromptTextTexture.loadFromRenderedText(gRenderer, "Recording...", gFont, gTextColor);
                                    currentState = RECORDING;
                                }
                            }
                            break;
                        case RECORDED:
                            printf("Recording done!\n");
                            //On key press
                            if (e.type == SDL_KEYDOWN) {
                                //Start playback
                                if (e.key.keysym.sym == SDLK_1) {
                                    //Go back to beginning of buffer
                                    gBufferBytePosition = 0;
                                    //Start playback
                                    SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);
                                    //Go on to next state
                                    gPromptTextTexture.loadFromRenderedText(gRenderer, "Playing...", gFont, gTextColor);
                                    currentState = PLAYBACK;
                                }
                                //Record again
                                if (e.key.keysym.sym == SDLK_2) {
                                    //Reset the buffer
                                    gBufferBytePosition = 0;
                                    memset(gRecordingBuffer, 0, gBufferByteSize);

                                    //Start recording
                                    SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);

                                    //Go on to next state
                                    gPromptTextTexture.loadFromRenderedText(gRenderer, "Recording...", gFont, gTextColor);
                                    currentState = RECORDING;
                                }
                            }
                            break;
                    }

                    // updating recording
                    if (currentState == RECORDING) {
                        printf("Recording...\n");
                        // lock callback
                        SDL_LockAudioDevice(recordingDeviceId);
                        printf("gBufferBytePosition: %d / %d\n", gBufferBytePosition, gBufferByteMaxPosition);
                        // finished recording
                        if (gBufferBytePosition > gBufferByteMaxPosition) {
                            // stop recording audio
                            SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);
                            // go on next state
                            gPromptTextTexture.loadFromRenderedText(gRenderer, "Press 1 to play back. Press 2 to record again", gFont, gTextColor);
                            currentState = RECORDED;
                        }
                        // unlock callback
                        SDL_UnlockAudioDevice(recordingDeviceId);
                    } else if (currentState == PLAYBACK) {
                        printf("PLAYBACK...\n");
                        // lock callback
                        SDL_LockAudioDevice(playbackDeviceId);
                        // finished playback
                        if (gBufferBytePosition > gBufferByteMaxPosition) {
                            // stop playback audio
                            SDL_PauseAudioDevice(playbackDeviceId, SDL_TRUE);
                            // go on next state
                            gPromptTextTexture.loadFromRenderedText(gRenderer, "Press 1 to play back. Press 2 to record again", gFont, gTextColor);
                            currentState = RECORDED;
                        }
                        // unlock callback
                        SDL_UnlockAudioDevice(playbackDeviceId);
                    }

                    // clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);

                    // render text textures
                    gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);

                    // user is selecting
                    if (currentState == SELECTING_DEVICE) {
                        // render device names
                        int yOffset = gPromptTextTexture.getHeight() * 2;
                        for (int i = 0; i < gRecordingDeviceCount; ++i) {
                            gDeviceTextures[i].render(gRenderer, 0, yOffset, NULL, NULL, NULL, SDL_FLIP_NONE);
                            yOffset += gDeviceTextures[i].getHeight() + 1;
                        }
                    }

                    // update screen
                    SDL_RenderPresent(gRenderer);
                }
            }
        }

        //Free resources and close SDL
        close();

        return 0;
    }
}