#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

// Function to play an MP3 file
void play_mp3(const char *filename);

// Playback control functions
void pause_playback(void);
void resume_playback(void);
void stop_playback(void);
void skip_to_next(void);
int is_playing(void);

// Volume control
void set_volume(int volume); // 0-100
int get_volume(void);

#endif
