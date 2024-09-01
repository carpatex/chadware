CC := cc
CFLAGS := -Wall -g
INCLUDE := -I.
OBJECTS_COMMON := chadware.o
OBJECTS_CLIENT := $(OBJECTS_COMMON) client.o
OBJECTS_DEBUG := $(OBJECTS_COMMON) debug.o
LIBS_CLIENT := -lncurses -ltinfo

all: client debug_print

.o: .c
	$(CC) $(INCLUDE) $(CFLAGS) -c $(input) -o $(output)

client: $(OBJECTS_CLIENT) 
	$(CC) $(OBJECTS_CLIENT) -o chadware $(LIBS_CLIENT)

debug_print: $(OBJECTS_DEBUG)
	$(CC) $(OBJECTS_DEBUG) -o debug_print 

clean:
	rm -rf *.o chadware chadserver debug_print
