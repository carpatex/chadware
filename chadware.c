#include "chadware.h"
static const GLOBALVAR_PROPORTION = 5;
static const DATAIN_PROPORTION = 10;
static const DATAOUT_PROPORTION = 10;
static const CHUNK_PROPORTION = 35;
/* static const ENTITY_PROPORTION = 40; */ 
/* este no va porque es al 
	 pedo ya que al ser el último se lleva el resto de la memoria
	 por descarte. */

static void *heap;
static size_t *globalvar_ptr, *datain_ptr, *dataout_ptr, *chunk_ptr, *entity_ptr,*heapsize_ptr; 
/* punteros a las primeras cinco variables de la heap, que marcan 
	 donde carajo empieza cada una de esas secciones. el último, 
	 heapsize_ptr, marca el final de la heap y por consiguiente, 
	 el tamaño de la misma */

void* init_heap_chadware(const size_t heap_size){
	if (((GLOBAL_VAR_PROPORTION * heap_size) / 100) < (5 * sizeof(size_t))) {
		return NULL;
	} else {
		heap = calloc(1, heap_size);

		heapsize_ptr = heap;
		globalvar_ptr = heap + (1 * sizeof(size_t));
		datain_ptr = heap + (2 * sizeof(size_t));
		dataout_ptr = heap + (3 * sizeof(size_t));
		chunk_ptr = heap + (4 * sizeof(size_t));
		entity_ptr = heap + (5 * sizeof(size_t));
		
		*heapsize_ptr = heap_size;
		*globalvar_ptr = heap;
		*datain_ptr = heap + ((GLOBALVAR_PROPORTION * heap_size) / 100);
		*dataout_ptr = *datain_ptr + ((DATAIN_PROPORTION * heap_size) / 100);
		*chunk_ptr = *dataout_ptr + ((DATAOUT_PROPORTION * heap_size) / 100);
		*entity_ptr = *chunk_ptr + ((CHUNK_PROPORTION * heap_size) / 100);
		/* se marca cuanta memoria usará cada seccion. 
			 el cálculo consiste en multiplicar el tamaño proporcional
			 que se les dedicará a cada seccion por el tamaño 
			 designado para la heap, todo esto dividido por 100, 
			 ya que es un porcentaje. como los bytes no se pueden 
			 subdividir en fracciones por cuestiones obvias, las 
			 cantidades que le tocan a cada uno son inexactas. 
			 para hacerlo más facil, el último, que es el que 
			 tiene mayor prioridad, se le da el resto de la memoria. */
		return heap;
	}
}

int tick(int n_op, int* op) {
	return 0;
}
