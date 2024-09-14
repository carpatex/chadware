#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_SIZE 16

struct packed_values {
	int32_t value;
	int8_t start_pos_x;
	int8_t start_pos_y;
	int8_t end_pos_x;
	int8_t end_pos_y;
};

// Función para buscar coincidencias y "comprimir" los valores
struct packed_values* compress_array(int32_t **arr, int* num_values) {
	struct packed_values* result;
	int count;

	result = (struct packed_values*) calloc(MAX_SIZE * MAX_SIZE, sizeof(struct packed_values));
	count = 0;

	int i, j;
	for (i = 0; i < MAX_SIZE; i++) {
		for (j = 0; j < MAX_SIZE; j++) {
			if (i == 0 || j == 0 || arr[i][j] != arr[i-1][j] || arr[i][j] != arr[i][j-1]) {
				int start_x, start_y, end_x, end_y;
				int32_t value;

				start_x = i;
				start_y = j;
				end_x = i;
				end_y = j;
				value = arr[i][j];

				// Buscar coincidencias hacia abajo y derecha
				while (end_x < MAX_SIZE && arr[end_x][j] == value) end_x++;
				while (end_y < MAX_SIZE && arr[i][end_y] == value) end_y++;

				// Añadir coincidencia a resultado
				result[count].value = value;
				result[count].start_pos_x = start_x;
				result[count].start_pos_y = start_y;
				result[count].end_pos_x = end_x - 1;
				result[count].end_pos_y = end_y - 1;
				count++;
			}
		}
	}

	*num_values = count;
	return result;
}

// Función para "descomprimir" los valores y llenar un arreglo bidimensional
void decompress_array(struct packed_values* packed_values, int n_packed_values, int32_t **arr){
    int i;
    for (i = 0; i < n_packed_values; i++) {
		int start_x = packed_values[i].start_pos_x;
		int start_y = packed_values[i].start_pos_y;
		int end_x = packed_values[i].end_pos_x;
		int end_y = packed_values[i].end_pos_y;
		int32_t value = packed_values[i].value;

		int x, y;
		for (x = start_x; x <= end_x; x++) {
			for (y = start_y; y <= end_y; y++) {
				arr[x][y] = value;
			}
		}
	}
}

int main() {
	int32_t **arr;
	int num_values;
  struct packed_values *r1;
	struct packed_values result[4] = { {},
    {},
    {},
    {},
  };
	// Inicializar arr con valores
	r1 = compress_array(arr, &num_values);

	// Imprimir resultado
	int i;
	for (i = 0; i < num_values; i++) {
		printf("Valor: %d, Posición inicial: (%d, %d), Posición final: (%d, %d)\n",
			   r1[i].value, r1[i].start_pos_x, r1[i].start_pos_y,
			   r1[i].end_pos_x, r1[i].end_pos_y);
	}
	free(r1);
	return 0;
}
