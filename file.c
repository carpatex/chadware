#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_BUFFER_SIZE 512

int main() {
	int i, num_a_reservar;
	int *numeros;
	char input_buffer[INPUT_BUFFER_SIZE];
	char filename[INPUT_BUFFER_SIZE];
	FILE *archivoni;
	puts("Cuantos números queres guardar en el archivo: ");
	fgets(input_buffer, INPUT_BUFFER_SIZE, stdin);
	if ( sscanf(input_buffer, "%i", &num_a_reservar) != 1 ) {
		perror("Carpa sos un gaga tenias que poner un número solamente virgooooo. salgo del prograna  con error porque me da paja volver a pedir el número");
		return EXIT_FAILURE;
	}
	if ( num_a_reservar < 1 ) {
		puts("No hay que hacer nada, pusiste un número menor a 1, hasta la proximaaa \n");
	}
	else {
		numeros = (int *) calloc((size_t) num_a_reservar, sizeof(int));
		if ( numeros == NULL ) {
			perror("Falló la reserva de memoria");
			return EXIT_FAILURE;
		}
		for ( i = 0; i < num_a_reservar; i++ ) {
			do {
				memset(input_buffer, 0, sizeof(input_buffer));
				printf("Contenido del número %i: ", i + 1);
				fgets(input_buffer, INPUT_BUFFER_SIZE - 1, stdin);
			} while ( sscanf(input_buffer, "%i", (numeros + sizeof(int) * i)) != 1 );
		}
		puts("Los números que pusiste son: ");
		for (i = 0; i < num_a_reservar; i++) {
			printf("%i\n", *(numeros + i * sizeof(int)));
		}



		memset(input_buffer, 0, sizeof(input_buffer));
		puts("Ahora el nombre del archivo donde lo vas a guardar: ");
		fgets(input_buffer, INPUT_BUFFER_SIZE, stdin);
		if ( sscanf(input_buffer, "%s", filename) != 1 ) {
			perror("Carpa sos un gaga tenias que poner un número solamente virgooooo. salgo del prograna  con error porque me da paja volver a pedir el número");
			return EXIT_FAILURE;
		}
		archivoni = fopen(filename, "w");
		if ( archivoni == NULL ) {
			perror("MALA NOTICIA MI GENTE, MALA NOTICIAS, NO HEMO PODIDO ABRIR EL ARCHIVO");
			return EXIT_FAILURE;
		}
		puts("guardando datos... ");
		for (i = 0; i < num_a_reservar; i++) {
			printf("%i\n", *(numeros + i * sizeof(int)) );
			fprintf(archivoni, "%i\n", *(numeros + i * sizeof(int)) );
		}
		fclose(archivoni);
		puts("lo' dato' an sido guardado satifatoriamente deaaaa  ");
		free(numeros);
	}
	return 0;
}

