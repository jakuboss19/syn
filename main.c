// main.c
#include "audio_player.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define MAX_SONGS 100
#define MAX_PATH_LENGTH 256

int get_songs_from_directory(char *songs[], int max_count) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    
    dir = opendir("songs");
    if (dir == NULL) {
        printf("Error: Could not open 'songs' directory. Please create it and add MP3 files.\n");
        return 0;
    }
    
    while ((ent = readdir(dir)) != NULL && count < max_count) {
        if (ent->d_type == DT_REG) { // Regular file
            char *filename = ent->d_name;
            if (strstr(filename, ".mp3") != NULL) {
                songs[count] = malloc(MAX_PATH_LENGTH);
                snprintf(songs[count], MAX_PATH_LENGTH, "songs/%s", filename);
                count++;
            }
        }
    }
    
    closedir(dir);
    return count;
}

void free_songs_array(char *songs[], int count) {
    for (int i = 0; i < count; i++) {
        free(songs[i]);
    }
}

void display_menu(void) {
    printf("\n=== MP3 Player Menu ===\n");
    printf("1. Play a song\n");
    printf("2. View play history\n");
    printf("3. Clear play history\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

void play_song_menu(char *songs[], int count) {
    if (count == 0) {
        printf("No MP3 files found in 'songs' directory.\n");
        printf("Please create a 'songs' directory and add some MP3 files.\n");
        return;
    }

    printf("\nAvailable songs:\n");
    for (int i = 0; i < count; i++) {
        // Extract just the filename without path
        char *filename = strrchr(songs[i], '/');
        if (filename) filename++; // Skip the '/'
        else filename = songs[i];
        printf("%d. %s\n", i + 1, filename);
    }

    int choice;
    printf("Enter the number of the song to play: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        return;
    }
    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return;
    }

    const char *filename = songs[choice - 1];
    play_mp3(filename);
    update_history(filename);
}

int main() {
    char *songs[MAX_SONGS];
    int count = get_songs_from_directory(songs, MAX_SONGS);
    
    if (count == 0) {
        printf("No MP3 files found in 'songs' directory.\n");
        printf("Please create a 'songs' directory and add some MP3 files.\n");
        printf("Then run the program again.\n");
        return 1;
    }

    int choice;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                play_song_menu(songs, count);
                break;
            case 2:
                read_history();
                break;
            case 3:
                clear_history();
                break;
            case 4:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter 1-4.\n");
                break;
        }
    } while (choice != 4);
    
    free_songs_array(songs, count);
    return 0;
}
