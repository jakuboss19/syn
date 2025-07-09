# syn

A simple command-line MP3 player written in C, using mpg123 for decoding and SDL2 for audio playback.

## Features
- Play MP3 files from a selectable list
- Logs played songs to `history.txt`

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
3. Select a song by entering its number.

## Notes
- Played songs are logged to `history.txt` in the current directory.
- To clean up build files, run:
  ```sh
  make clean
  ```