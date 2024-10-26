CC := cc
CFLAGS := -Wall -g
INCLUDE := -I.
OBJECTS_COMMON := chadware.o entity.o world.o worldgen.o
OBJECTS_CLIENT := $(OBJECTS_COMMON) client.o
LIBS_CLIENT := -lncurses -ltinfo -lm

all: client 

.o: .c
	$(CC) $(INCLUDE) $(CFLAGS) -c $(input) -o $(output)

client: $(OBJECTS_CLIENT) 
	$(CC) $(OBJECTS_CLIENT) -o chadware $(LIBS_CLIENT)

clean:
	rm -rf *.o chadware chadserver debug_print
