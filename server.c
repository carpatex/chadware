#include "chadware.h"
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 12345
#define MAX_CONN 10
#define BUFFER_SIZE 1024

// Definimos un enum para los posibles estados de un cliente
typedef enum {
	WAITING_FOR_DATA,
	DATA_RECEIVED,
	DATA_ECHOED
} client_state_t;

// Estructura para almacenar la información de cada cliente
typedef struct {
	int socket;
	client_state_t state;
	unsigned char buffer[BUFFER_SIZE];
	int message_count;  // Contador de mensajes por cliente
} client_info_t;

int main() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len = sizeof(client_addr);
	fd_set master_set, read_fds;
	int max_fd, i, bytes_received;

	// Crear el socket del servidor
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error al crear el socket");
		exit(EXIT_FAILURE);
	}

	// Configurar la dirección del servidor
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Enlazar el socket con la dirección
	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("Error al enlazar el socket");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// Escuchar por conexiones entrantes
	if (listen(server_socket, MAX_CONN) == -1) {
		perror("Error al escuchar en el socket");
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// Inicializar el conjunto de descriptores de archivo
	FD_ZERO(&master_set);
	FD_SET(server_socket, &master_set);
	max_fd = server_socket;

	printf("Servidor escuchando en el puerto %d...\n", PORT);

	// Arreglo para almacenar la información de cada cliente
	client_info_t clients[MAX_CONN];

	while (1) {
		read_fds = master_set;

		// Usamos select para esperar actividad en cualquier socket
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("Error en select");
			break;
		}

		// Recorrer todos los sockets para ver cuáles tienen actividad
		for (i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &read_fds)) {
				// Si es el socket del servidor, significa que hay un cliente nuevo
				if (i == server_socket) {
					client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
					if (client_socket == -1) {
						perror("Error al aceptar la conexión");
						continue;
					}

					FD_SET(client_socket, &master_set);
					if (client_socket > max_fd) {
						max_fd = client_socket;
					}

					// Inicializamos la información del cliente
					clients[client_socket].socket = client_socket;
					clients[client_socket].state = WAITING_FOR_DATA;
					clients[client_socket].message_count = 0;  // Inicializamos el contador de mensajes
					printf("Nuevo cliente conectado\n");
				} else {
					// Si no es el socket del servidor, es un cliente ya conectado
					client_info_t *client = &clients[i];

					bytes_received = recv(i, client->buffer, sizeof(client->buffer), 0);
					if (bytes_received <= 0) {
						// Si no se recibieron datos o se desconectó el cliente
						if (bytes_received == 0) {
							printf("Cliente %d desconectado\n", i);
						} else {
							perror("Error al recibir datos");
						}
						close(i);
						FD_CLR(i, &master_set);
					} else {
						// Incrementamos el contador de mensajes del cliente
						client->message_count++;

						// Procesar los datos recibidos según el estado del cliente
						if (client->state == WAITING_FOR_DATA) {
							printf("Cliente %d, Mensaje %d: Recibidos %d bytes de datos binarios\n", i, client->message_count, bytes_received);
							client->state = DATA_RECEIVED;  // El cliente ahora está en el estado de datos recibidos
						} else if (client->state == DATA_RECEIVED) {
							printf("Cliente %d, Mensaje %d: Recibidos %d bytes más de datos binarios\n", i, client->message_count, bytes_received);
							client->state = DATA_ECHOED;  // El cliente ahora está en el estado de datos enviados (eco)
						}

						// Enviar los datos de vuelta al cliente (eco)
						if (send(i, client->buffer, bytes_received, 0) == -1) {
							perror("Error al enviar datos al cliente");
							close(i);
							FD_CLR(i, &master_set);
						}
					}
				}
			}
		}
	}

	close(server_socket);
	return 0;
}
