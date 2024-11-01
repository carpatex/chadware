CC := cc
CFLAGS := -Wall -g
INCLUDE := -I.
OBJECTS_COMMON := chadware.o entity.o world.o worldgen.o
OBJECTS_T_CHUNK := $(OBJECTS_COMMON) test_chunk.o
OBJECTS_CLIENT := $(OBJECTS_COMMON) client.o
LIBS_CLIENT := -lncurses -ltinfo -lm

all: client 

.o: .c
	$(CC) $(INCLUDE) $(CFLAGS) -c $(input) -o $(output)

client: $(OBJECTS_CLIENT) 
	$(CC) $(OBJECTS_CLIENT) -o chadware $(LIBS_CLIENT)

test_chunk: $(OBJECTS_T_CHUNK) 
	$(CC) $(OBJECTS_T_CHUNK) -o test_chunk 

clean:
	rm -rf *.o chadware chadserver debug_print test_chunk
