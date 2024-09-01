CC := cc
CFLAGS := -Wall -g
INCLUDE := -I.
OBJECTS_COMMON := chadware.o
OBJECTS_CLIENT := $(OBJECTS_COMMON) client.o
LIBS_CLIENT := -lncurses -ltinfo


client: $(OBJECTS_CLIENT) 
	$(CC) $(inputs) -o chadware $(LIBS_CLIENT)
%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $(input) -o $(output)

clean:
	rm -rf *.o chadware chadserver debug_print
