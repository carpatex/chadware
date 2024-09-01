#include "chadware.h"

int main() {
	init_heap_chadware(1024*128);
	debug_print_ptrs();
	return 0;
}
