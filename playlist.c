#include "playlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Playlist* create_playlist(const char *name) {
    Playlist *playlist = malloc(sizeof(Playlist));
    if (playlist == NULL) {
        fprintf(stderr, "Failed to allocate memory for playlist\n");
        return NULL;
    }
    
    strncpy(playlist->name, name, sizeof(playlist->name) - 1);
    playlist->name[sizeof(playlist->name) - 1] = '\0';
    playlist->count = 0;
    
    printf("Playlist '%s' created successfully.\n", name);
    return playlist;
}

void add_song_to_playlist(Playlist *playlist, const char *filename) {
    if (playlist == NULL || filename == NULL) {
        printf("Invalid playlist or filename.\n");
        return;
    }
    
    if (playlist->count >= MAX_PLAYLIST_SIZE) {
        printf("Playlist is full. Cannot add more songs.\n");
        return;
    }
    
    strncpy(playlist->songs[playlist->count], filename, MAX_FILENAME_LENGTH - 1);
    playlist->songs[playlist->count][MAX_FILENAME_LENGTH - 1] = '\0';
    playlist->count++;
    
    // Extract just the filename for display
    char *filename_only = strrchr(filename, '/');
    if (filename_only) filename_only++; // Skip the '/'
    else filename_only = (char*)filename;
    
    printf("Added '%s' to playlist '%s'.\n", filename_only, playlist->name);
}

void remove_song_from_playlist(Playlist *playlist, int index) {
    if (playlist == NULL) {
        printf("Invalid playlist.\n");
        return;
    }
    
    if (index < 1 || index > playlist->count) {
        printf("Invalid song index.\n");
        return;
    }
    
    // Convert to 0-based index
    index--;
    
    // Extract filename for display before removal
    char *filename_only = strrchr(playlist->songs[index], '/');
    if (filename_only) filename_only++; // Skip the '/'
    else filename_only = playlist->songs[index];
    
    // Shift remaining songs
    for (int i = index; i < playlist->count - 1; i++) {
        strcpy(playlist->songs[i], playlist->songs[i + 1]);
    }
    
    playlist->count--;
    printf("Removed '%s' from playlist '%s'.\n", filename_only, playlist->name);
}

void save_playlist(const Playlist *playlist, const char *filename) {
    if (playlist == NULL || filename == NULL) {
        printf("Invalid playlist or filename.\n");
        return;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to create playlist file '%s'.\n", filename);
        return;
    }
    
    // Write playlist header
    fprintf(file, "# Playlist: %s\n", playlist->name);
    fprintf(file, "# Created: %s", ctime(&(time_t){time(NULL)}));
    fprintf(file, "# Total songs: %d\n\n", playlist->count);
    
    // Write songs
    for (int i = 0; i < playlist->count; i++) {
        fprintf(file, "%s\n", playlist->songs[i]);
    }
    
    fclose(file);
    printf("Playlist '%s' saved to '%s'.\n", playlist->name, filename);
}

Playlist* load_playlist(const char *filename) {
    if (filename == NULL) {
        printf("Invalid filename.\n");
        return NULL;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open playlist file '%s'.\n", filename);
        return NULL;
    }
    
    char line[512];
    char playlist_name[64] = "Unknown";
    int song_count = 0;
    
    // Read playlist header
    while (fgets(line, sizeof(line), file) && line[0] == '#') {
        if (strncmp(line, "# Playlist:", 11) == 0) {
            sscanf(line, "# Playlist: %[^\n]", playlist_name);
        }
    }
    
    // Create playlist
    Playlist *playlist = create_playlist(playlist_name);
    if (playlist == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Add the first song (from the line we just read)
    if (line[0] != '#' && strlen(line) > 1) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        add_song_to_playlist(playlist, line);
        song_count++;
    }
    
    // Read remaining songs
    while (fgets(line, sizeof(line), file) && song_count < MAX_PLAYLIST_SIZE) {
        if (line[0] != '#' && strlen(line) > 1) {
            line[strcspn(line, "\n")] = 0; // Remove newline
            add_song_to_playlist(playlist, line);
            song_count++;
        }
    }
    
    fclose(file);
    printf("Loaded playlist '%s' with %d songs.\n", playlist->name, playlist->count);
    return playlist;
}

void display_playlist(const Playlist *playlist) {
    if (playlist == NULL) {
        printf("Invalid playlist.\n");
        return;
    }
    
    printf("\n=== Playlist: %s ===\n", playlist->name);
    printf("Total songs: %d\n\n", playlist->count);
    
    if (playlist->count == 0) {
        printf("Playlist is empty.\n");
        return;
    }
    
    for (int i = 0; i < playlist->count; i++) {
        // Extract just the filename without path
        char *filename = strrchr(playlist->songs[i], '/');
        if (filename) filename++; // Skip the '/'
        else filename = (char*)playlist->songs[i];
        
        printf("%d. %s\n", i + 1, filename);
    }
    printf("\n");
}

void free_playlist(Playlist *playlist) {
    if (playlist != NULL) {
        free(playlist);
        printf("Playlist freed from memory.\n");
    }
}

void shuffle_playlist(Playlist *playlist) {
    if (playlist == NULL || playlist->count <= 1) {
        printf("Cannot shuffle: playlist is empty or has only one song.\n");
        return;
    }
    
    srand(time(NULL));
    
    for (int i = playlist->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // Swap songs
        char temp[MAX_FILENAME_LENGTH];
        strcpy(temp, playlist->songs[i]);
        strcpy(playlist->songs[i], playlist->songs[j]);
        strcpy(playlist->songs[j], temp);
    }
    
    printf("Playlist '%s' shuffled successfully.\n", playlist->name);
}

void sort_playlist_by_name(Playlist *playlist) {
    if (playlist == NULL || playlist->count <= 1) {
        printf("Cannot sort: playlist is empty or has only one song.\n");
        return;
    }
    
    // Simple bubble sort
    for (int i = 0; i < playlist->count - 1; i++) {
        for (int j = 0; j < playlist->count - i - 1; j++) {
            if (strcmp(playlist->songs[j], playlist->songs[j + 1]) > 0) {
                // Swap songs
                char temp[MAX_FILENAME_LENGTH];
                strcpy(temp, playlist->songs[j]);
                strcpy(playlist->songs[j], playlist->songs[j + 1]);
                strcpy(playlist->songs[j + 1], temp);
            }
        }
    }
    
    printf("Playlist '%s' sorted alphabetically.\n", playlist->name);
} 