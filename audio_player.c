#include "audio_player.h"
#include <mpg123.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void play_mp3(const char *filename) {
    mpg123_handle *mh = NULL;
    unsigned char *buffer;
    size_t done;
    int err;

    int channels, encoding;
    long rate;

    // Initialize mpg123 library
    if (mpg123_init() != MPG123_OK) {
        fprintf(stderr, "Failed to initialize mpg123.\n");
        return;
    }

    // Create a mpg123 handle
    mh = mpg123_new(NULL, &err);
    if (mh == NULL) {
        fprintf(stderr, "Unable to create mpg123 handle: %s\n", mpg123_plain_strerror(err));
        return;
    }

    // Open the MP3 file
    if (mpg123_open(mh, filename) != MPG123_OK) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        goto cleanup;
    }

    // Get audio format
    if (mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK) {
        fprintf(stderr, "Failed to get audio format.\n");
        goto cleanup;
    }

    // Initialize SDL audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        goto cleanup;
    }

    SDL_AudioSpec wanted_spec;
    wanted_spec.freq = (int)rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = (Uint8)channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = 1024;
    wanted_spec.callback = NULL;

    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        SDL_Quit();
        goto cleanup;
    }

    // Start playback
    SDL_PauseAudio(0);

    buffer = (unsigned char*) malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        goto cleanup_sdl;
    }

    printf("Playing: %s\n", filename);

    while (mpg123_read(mh, buffer, BUFFER_SIZE, &done) == MPG123_OK) {
        SDL_QueueAudio(1, buffer, (Uint32)done);
        SDL_Delay(10); // Short delay to allow playback
    }

    // Wait for all audio to finish
    while (SDL_GetQueuedAudioSize(1) > 0) {
        SDL_Delay(100);
    }

    free(buffer);

cleanup_sdl:
    SDL_CloseAudio();
    SDL_Quit();

cleanup:
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}
