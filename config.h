#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int default_volume;
    char default_playlist[64];
    int auto_play_next;
    int show_visualizer;
    char songs_directory[256];
    char history_file[256];
} Config;

// Configuration management
Config* load_config(const char *filename);
void save_config(const Config *config, const char *filename);
void set_default_config(Config *config);
void print_config(const Config *config);

// Configuration getters/setters
int get_default_volume(const Config *config);
void set_default_volume(Config *config, int volume);
const char* get_songs_directory(const Config *config);
void set_songs_directory(Config *config, const char *directory);

#endif 