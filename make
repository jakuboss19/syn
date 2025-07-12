CC = gcc
CFLAGS = -Wall -std=c11
LIBS = -lmpg123 -lSDL2
OBJ = main.o audio_player.o history.o playlist.o

all: player

player: $(OBJ)
	$(CC) $(CFLAGS) -o player $(OBJ) $(LIBS)

clean:
	rm -f *.o player
