#ifndef PLAYLIST_H
#define PLAYLIST_H

#define MAX_PLAYLIST_SIZE 100
#define MAX_FILENAME_LENGTH 256

typedef struct {
    char songs[MAX_PLAYLIST_SIZE][MAX_FILENAME_LENGTH];
    int count;
    char name[64];
} Playlist;

// Playlist management
Playlist* create_playlist(const char *name);
void add_song_to_playlist(Playlist *playlist, const char *filename);
void remove_song_from_playlist(Playlist *playlist, int index);
void save_playlist(const Playlist *playlist, const char *filename);
Playlist* load_playlist(const char *filename);
void display_playlist(const Playlist *playlist);
void free_playlist(Playlist *playlist);

// Playlist operations
void shuffle_playlist(Playlist *playlist);
void sort_playlist_by_name(Playlist *playlist);

#endif 