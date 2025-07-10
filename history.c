#include "history.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void update_history(const char *filename) {
    FILE *file = fopen("history.txt", "a");
    if (file) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Remove newline
        
        // Extract just the filename without path
        char *filename_only = strrchr(filename, '/');
        if (filename_only) filename_only++; // Skip the '/'
        else filename_only = (char*)filename;
        
        fprintf(file, "[%s] %s\n", time_str, filename_only);
        fclose(file);
    }
}

void read_history(void) {
    FILE *file = fopen("history.txt", "r");
    if (!file) {
        printf("No history found.\n");
        return;
    }
    
    printf("\n=== Play History ===\n");
    char line[512];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        count++;
    }
    printf("=== Total: %d entries ===\n\n", count);
    fclose(file);
}

void clear_history(void) {
    if (remove("history.txt") == 0) {
        printf("History cleared successfully.\n");
    } else {
        printf("Failed to clear history.\n");
    }
} 