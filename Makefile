CC			= gcc -o
#CFLAGS	= -Wall

emulator: emulator.c
	$(CC) emulator $(CFLAGS) emulator.c



