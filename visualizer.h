#ifndef VISUALIZER_H
#define VISUALIZER_H

// Audio visualization functions
void init_visualizer(void);
void update_spectrum(const unsigned char *audio_data, size_t data_size);
void draw_spectrum(void);
void cleanup_visualizer(void);

// Visualization modes
typedef enum {
    VISUALIZER_BARS,
    VISUALIZER_WAVEFORM,
    VISUALIZER_SPECTRUM
} VisualizerMode;

void set_visualizer_mode(VisualizerMode mode);
VisualizerMode get_visualizer_mode(void);

#endif 