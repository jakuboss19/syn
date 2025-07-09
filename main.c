// main.c
#include "audio_player.h"
#include "history.h"
#include <stdio.h>

int main() {
    char *songs[] = {
        "songs/song1.mp3",
        "songs/song2.mp3"
    };
    int count = sizeof(songs) / sizeof(songs[0]);

    printf("Available songs:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, songs[i]);
    }

    int choice;
    printf("Enter the number of the song to play: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        return 1;
    }
    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        return 1;
    }

    const char *filename = songs[choice - 1];
    play_mp3(filename);
    update_history(filename);
    return 0;
}
