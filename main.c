// main.c
#include "audio_player.h"
#include "history.h"
#include "playlist.h"
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
    printf("2. Playback controls\n");
    printf("3. Volume control\n");
    printf("4. Playlist management\n");
    printf("5. View play history\n");
    printf("6. Clear play history\n");
    printf("7. Exit\n");
    printf("Enter your choice (1-7): ");
}

void playlist_management_menu(char *songs[], int count) {
    static Playlist *current_playlist = NULL;
    
    printf("\n=== Playlist Management ===\n");
    printf("1. Create new playlist\n");
    printf("2. Load existing playlist\n");
    printf("3. Save current playlist\n");
    printf("4. Display current playlist\n");
    printf("5. Add song to playlist\n");
    printf("6. Remove song from playlist\n");
    printf("7. Shuffle playlist\n");
    printf("8. Sort playlist\n");
    printf("9. Play playlist\n");
    printf("10. Back to main menu\n");
    printf("Enter your choice (1-10): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return;
    }
    
    char playlist_name[64];
    char filename[256];
    
    switch (choice) {
        case 1:
            printf("Enter playlist name: ");
            scanf("%63s", playlist_name);
            if (current_playlist) {
                free_playlist(current_playlist);
            }
            current_playlist = create_playlist(playlist_name);
            break;
            
        case 2:
            printf("Enter playlist filename: ");
            scanf("%255s", filename);
            if (current_playlist) {
                free_playlist(current_playlist);
            }
            current_playlist = load_playlist(filename);
            break;
            
        case 3:
            if (current_playlist) {
                printf("Enter filename to save: ");
                scanf("%255s", filename);
                save_playlist(current_playlist, filename);
            } else {
                printf("No playlist loaded.\n");
            }
            break;
            
        case 4:
            if (current_playlist) {
                display_playlist(current_playlist);
            } else {
                printf("No playlist loaded.\n");
            }
            break;
            
        case 5:
            if (current_playlist && count > 0) {
                printf("Available songs:\n");
                for (int i = 0; i < count; i++) {
                    char *filename_only = strrchr(songs[i], '/');
                    if (filename_only) filename_only++;
                    else filename_only = songs[i];
                    printf("%d. %s\n", i + 1, filename_only);
                }
                
                int song_choice;
                printf("Enter song number to add: ");
                if (scanf("%d", &song_choice) == 1 && song_choice >= 1 && song_choice <= count) {
                    add_song_to_playlist(current_playlist, songs[song_choice - 1]);
                } else {
                    printf("Invalid song choice.\n");
                }
            } else {
                printf("No playlist loaded or no songs available.\n");
            }
            break;
            
        case 6:
            if (current_playlist) {
                display_playlist(current_playlist);
                int remove_choice;
                printf("Enter song number to remove: ");
                if (scanf("%d", &remove_choice) == 1) {
                    remove_song_from_playlist(current_playlist, remove_choice);
                } else {
                    printf("Invalid choice.\n");
                }
            } else {
                printf("No playlist loaded.\n");
            }
            break;
            
        case 7:
            if (current_playlist) {
                shuffle_playlist(current_playlist);
            } else {
                printf("No playlist loaded.\n");
            }
            break;
            
        case 8:
            if (current_playlist) {
                sort_playlist_by_name(current_playlist);
            } else {
                printf("No playlist loaded.\n");
            }
            break;
            
        case 9:
            if (current_playlist && current_playlist->count > 0) {
                printf("Playing playlist: %s\n", current_playlist->name);
                for (int i = 0; i < current_playlist->count; i++) {
                    printf("Playing %d/%d: %s\n", i + 1, current_playlist->count, current_playlist->songs[i]);
                    play_mp3(current_playlist->songs[i]);
                    update_history(current_playlist->songs[i]);
                }
            } else {
                printf("No playlist loaded or playlist is empty.\n");
            }
            break;
            
        case 10:
            return;
            
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void playback_controls_menu(void) {
    printf("\n=== Playback Controls ===\n");
    printf("1. Pause playback\n");
    printf("2. Resume playback\n");
    printf("3. Stop playback\n");
    printf("4. Check if playing\n");
    printf("5. Back to main menu\n");
    printf("Enter your choice (1-5): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return;
    }
    
    switch (choice) {
        case 1:
            pause_playback();
            break;
        case 2:
            resume_playback();
            break;
        case 3:
            stop_playback();
            break;
        case 4:
            if (is_playing()) {
                printf("Currently playing.\n");
            } else {
                printf("Not currently playing.\n");
            }
            break;
        case 5:
            return;
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void volume_control_menu(void) {
    printf("\n=== Volume Control ===\n");
    printf("Current volume: %d%%\n", get_volume());
    printf("1. Set volume (0-100)\n");
    printf("2. Increase volume (+10)\n");
    printf("3. Decrease volume (-10)\n");
    printf("4. Back to main menu\n");
    printf("Enter your choice (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return;
    }
    
    int current_vol = get_volume();
    
    switch (choice) {
        case 1:
            printf("Enter new volume (0-100): ");
            int new_vol;
            if (scanf("%d", &new_vol) == 1) {
                set_volume(new_vol);
            }
            break;
        case 2:
            set_volume(current_vol + 10);
            break;
        case 3:
            set_volume(current_vol - 10);
            break;
        case 4:
            return;
        default:
            printf("Invalid choice.\n");
            break;
    }
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
                playback_controls_menu();
                break;
            case 3:
                volume_control_menu();
                break;
            case 4:
                playlist_management_menu(songs, count);
                break;
            case 5:
                read_history();
                break;
            case 6:
                clear_history();
                break;
            case 7:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter 1-7.\n");
                break;
        }
    } while (choice != 7);
    
    free_songs_array(songs, count);
    return 0;
}
