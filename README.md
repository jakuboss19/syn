# syn

A feature-rich command-line MP3 player written in C, using mpg123 for decoding and SDL2 for audio playback.

## Features
- **Dynamic MP3 Discovery**: Automatically finds MP3 files in the `songs/` directory
- **Playback Controls**: Play, pause, resume, and stop functionality
- **Volume Control**: Adjustable volume from 0-100%
- **Playlist Management**: Create, save, load, and manage playlists
  - Add/remove songs from playlists
  - Shuffle and sort playlists
  - Play entire playlists sequentially
- **Enhanced History**: Timestamped play history with view/clear options
- **Interactive Menu System**: Easy-to-use command-line interface

## Dependencies
- [mpg123](https://www.mpg123.de/) (for MP3 decoding)
- [SDL2](https://www.libsdl.org/) (for audio output)

## Build Instructions

1. Install dependencies (on Ubuntu/Debian):
   ```sh
   sudo apt-get install libmpg123-dev libsdl2-dev
   ```
   On Windows, use vcpkg or download binaries for your compiler.

2. Build the project:
   ```sh
   make
   ```

## Usage

1. Place your MP3 files in a `songs/` directory (e.g., `songs/song1.mp3`).
2. Run the player:
   ```sh
   ./player
   ```

### Main Menu Options
1. **Play a song** - Select and play individual MP3 files
2. **Playback controls** - Pause, resume, stop, and check playback status
3. **Volume control** - Set, increase, or decrease volume
4. **Playlist management** - Full playlist functionality
5. **View play history** - See timestamped history of played songs
6. **Clear play history** - Remove all history entries
7. **Exit** - Quit the application

### Playlist Management
- Create new playlists with custom names
- Load existing playlist files
- Save playlists to files for later use
- Add songs from your library to playlists
- Remove songs from playlists
- Shuffle playlists for random playback
- Sort playlists alphabetically
- Play entire playlists sequentially

### Playback Controls
- **Pause**: Temporarily stop playback
- **Resume**: Continue from where you paused
- **Stop**: Completely stop playback and reset
- **Volume**: Adjust from 0% (mute) to 100% (full volume)

## File Structure
```
syn/
├── main.c              # Main application and menu system
├── audio_player.c      # MP3 playback and controls
├── audio_player.h      # Audio player function declarations
├── history.c           # Play history management
├── history.h           # History function declarations
├── playlist.c          # Playlist system implementation
├── playlist.h          # Playlist data structures and functions
├── make                # Build configuration
└── README.md           # This file
```

## Notes
- Played songs are logged to `history.txt` with timestamps
- Playlists are saved as text files with metadata
- The player automatically discovers MP3 files in the `songs/` directory
- To clean up build files, run:
  ```sh
  make clean
  ```

## Future Features
- Audio visualization
- Multi-format support (OGG, FLAC, WAV)
- Configuration system
- Keyboard shortcuts for playback control
- Equalizer and audio effects
