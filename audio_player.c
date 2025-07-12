#include "audio_player.h"
#include <mpg123.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

// Global state variables
static SDL_AudioDeviceID audio_device = 0;
static mpg123_handle *mpg123_handle = NULL;
static unsigned char *audio_buffer = NULL;
static int is_paused = 0;
static int is_stopped = 0;
static int current_volume = 100;
static char current_filename[256] = "";

void play_mp3(const char *filename) {
    // Stop any currently playing audio
    if (audio_device != 0) {
        stop_playback();
    }
    
    strncpy(current_filename, filename, sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';
    
    mpg123_handle = NULL;
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
    mpg123_handle = mpg123_new(NULL, &err);
    if (mpg123_handle == NULL) {
        fprintf(stderr, "Unable to create mpg123 handle: %s\n", mpg123_plain_strerror(err));
        return;
    }

    // Open the MP3 file
    if (mpg123_open(mpg123_handle, filename) != MPG123_OK) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        goto cleanup;
    }

    // Get audio format
    if (mpg123_getformat(mpg123_handle, &rate, &channels, &encoding) != MPG123_OK) {
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

    SDL_AudioSpec obtained_spec;
    SDL_zero(obtained_spec);
    
    // Open the default audio device
    audio_device = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &obtained_spec, 0);
    if (audio_device == 0) {
        fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
        SDL_Quit();
        goto cleanup;
    }

    // Start playback
    SDL_PauseAudioDevice(audio_device, 0);
    is_paused = 0;
    is_stopped = 0;

    audio_buffer = (unsigned char*) malloc(BUFFER_SIZE);
    if (audio_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        goto cleanup_sdl;
    }

    printf("Playing: %s\n", filename);
    printf("Controls: [P]ause, [R]esume, [S]top, [Q]uit\n");

    while (mpg123_read(mpg123_handle, audio_buffer, BUFFER_SIZE, &done) == MPG123_OK && !is_stopped) {
        if (!is_paused) {
            // Apply volume
            if (current_volume < 100) {
                for (size_t i = 0; i < done; i += 2) {
                    int16_t sample = (audio_buffer[i] | (audio_buffer[i+1] << 8));
                    sample = (sample * current_volume) / 100;
                    audio_buffer[i] = sample & 0xFF;
                    audio_buffer[i+1] = (sample >> 8) & 0xFF;
                }
            }
            
            SDL_QueueAudio(audio_device, audio_buffer, (Uint32)done);
        }
        SDL_Delay(10); // Short delay to allow playback
    }

    // Wait for all audio to finish
    while (SDL_GetQueuedAudioSize(audio_device) > 0 && !is_stopped) {
        SDL_Delay(100);
    }

    free(audio_buffer);
    audio_buffer = NULL;

cleanup_sdl:
    SDL_CloseAudioDevice(audio_device);
    audio_device = 0;
    SDL_Quit();

cleanup:
    mpg123_close(mpg123_handle);
    mpg123_delete(mpg123_handle);
    mpg123_handle = NULL;
    mpg123_exit();
    
    is_paused = 0;
    is_stopped = 0;
}

void pause_playback(void) {
    if (audio_device != 0 && !is_paused && !is_stopped) {
        is_paused = 1;
        printf("Playback paused.\n");
    }
}

void resume_playback(void) {
    if (audio_device != 0 && is_paused && !is_stopped) {
        is_paused = 0;
        printf("Playback resumed.\n");
    }
}

void stop_playback(void) {
    if (audio_device != 0) {
        is_stopped = 1;
        is_paused = 0;
        printf("Playback stopped.\n");
        
        // Clear the audio queue
        SDL_ClearQueuedAudio(audio_device);
        
        // Close audio device
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
        
        // Clean up mpg123
        if (mpg123_handle) {
            mpg123_close(mpg123_handle);
            mpg123_delete(mpg123_handle);
            mpg123_handle = NULL;
        }
        
        // Free buffer
        if (audio_buffer) {
            free(audio_buffer);
            audio_buffer = NULL;
        }
        
        SDL_Quit();
    }
}

void skip_to_next(void) {
    // This would be implemented when we add playlist functionality
    printf("Skip to next not implemented yet. Use playlist feature.\n");
}

int is_playing(void) {
    return (audio_device != 0 && !is_paused && !is_stopped);
}

void set_volume(int volume) {
    if (volume >= 0 && volume <= 100) {
        current_volume = volume;
        printf("Volume set to %d%%\n", volume);
    } else {
        printf("Volume must be between 0 and 100\n");
    }
}

int get_volume(void) {
    return current_volume;
}
