CC=cc
CFLAGS=-Wall -g
OBJECTS_COMMON = chadware.c
OBJECTS_CLIENT = $(OBJECTS_COMMON) client.c
LIBS_CLIENT = -lncurses

client: $(OBJECTS_COMMON) $(OBJECTS_CLIENT)
	$(CC) $(CFLAGS) $(OBJECTS_CLIENT) -o chadware

clean:
	rm -rf *.o chadware chadserver debug_print
