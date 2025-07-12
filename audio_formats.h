#ifndef AUDIO_FORMATS_H
#define AUDIO_FORMATS_H

typedef enum {
    FORMAT_MP3,
    FORMAT_OGG,
    FORMAT_FLAC,
    FORMAT_WAV,
    FORMAT_UNKNOWN
} AudioFormat;

// Format detection and support
AudioFormat detect_audio_format(const char *filename);
const char* get_format_extension(AudioFormat format);
int is_format_supported(AudioFormat format);

// Format-specific functions
void play_mp3_file(const char *filename);
void play_ogg_file(const char *filename);
void play_flac_file(const char *filename);
void play_wav_file(const char *filename);

// Universal play function
void play_audio_file(const char *filename);

#endif 