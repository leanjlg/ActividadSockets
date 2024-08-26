#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024
#define PORT 5000


char *generarNombreUsuario(int longitud) {
    static char nombre[16];
    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int esVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            nombre[i] = vocales[rand() % 5];
        } else {
            nombre[i] = consonantes[rand() % 21];
        }
        esVocal = !esVocal;  // Alternar entre vocales y consonantes
    }
    nombre[longitud] = '\0';
    return nombre;
}

char *generarContrasena(int longitud) {
    static char contrasena[51];
    char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
    return contrasena;
}

int validar_longitud(int longitud, int min, int max) {
    return longitud >= min && longitud <= max;
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    int client_len, recv_size;

    srand(time(0));

    printf("Iniciando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al iniciar winsock: %d", WSAGetLastError());
        return 1;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d", WSAGetLastError());
        return 1;
    }
    printf("Socket creado.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error en bind: %d", WSAGetLastError());
        return 1;
    }

    // Escuchar conexiones
    listen(server_socket, 3);
    printf("Esperando conexiones entrantes...\n");

    client_len = sizeof(struct sockaddr_in);

    // Aceptar conexiones
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Error al aceptar la conexion: %d", WSAGetLastError());
        return 1;
    }
    printf("Conexion aceptada.\n");

    // Interaccion con el cliente
    while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) != SOCKET_ERROR) {
        buffer[recv_size] = '\0';
        printf("Mensaje recibido: %s\n", buffer);

        char respuesta[BUFFER_SIZE];

        if (strncmp(buffer, "nombre", 6) == 0) {
            int longitud = atoi(buffer + 7);  // La longitud después de "nombre "
            if (validar_longitud(longitud, 5, 15)) {
                sprintf(respuesta, "Nombre de usuario: %s", generarNombreUsuario(longitud));
            } else {
                strcpy(respuesta, "Error: La longitud debe estar entre 5 y 15.");
            }
        } else if (strncmp(buffer, "contrasena", 10) == 0) {
            int longitud = atoi(buffer + 11);  // La longitud después de "contrasena "
            if (validar_longitud(longitud, 8, 50)) {
                sprintf(respuesta, "Contrasena: %s", generarContrasena(longitud));
            } else {
                strcpy(respuesta, "Error: La longitud debe estar entre 8 y 50.");
            }
        } else {
            strcpy(respuesta, "Opcion no valida.");
        }

        send(client_socket, respuesta, strlen(respuesta), 0);
        printf("Mensaje enviado: %s\n", respuesta);
    }

    if (recv_size == SOCKET_ERROR) {
        printf("Error al recibir datos: %d", WSAGetLastError());
    }

    // Cerrar sockets
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
