#include "history.h"
#include <stdio.h>

void update_history(const char *filename) {
    FILE *file = fopen("history.txt", "a");
    if (file) {
        fprintf(file, "%s\n", filename);
        fclose(file);
    }
} 