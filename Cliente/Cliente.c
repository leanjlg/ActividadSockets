#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

int main() {
    WSADATA wsa;
    SOCKET socket_desc;
    struct sockaddr_in server;
    char msj[BUFFER_SIZE], server_resp[BUFFER_SIZE];
    int recv_size;

    printf("Iniciando winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al iniciar winsock: %d", WSAGetLastError());
        return 1;
    }

    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d", WSAGetLastError());
        return 1;
    }
    printf("Socket creado.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // Conectar al servidor
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error al conectar.\n");
        return 1;
    }
    printf("Conectado al servidor.\n");

    // Menú para el usuario
    while (1) {
        printf("\nOpciones:\n");
        printf("1. Generar nombre de usuario\n");
        printf("2. Generar contrasena\n");
        printf("3. Salir\n");
        printf("Elija una opcion: ");
        int opcion;
        scanf("%d", &opcion);

        if (opcion == 1) {
            int longitud;
            printf("Ingrese la longitud del nombre de usuario (5-15): ");
            scanf("%d", &longitud);
            sprintf(msj, "nombre %d", longitud);
        } else if (opcion == 2) {
            int longitud;
            printf("Ingrese la longitud de la contrasena (8-50): ");
            scanf("%d", &longitud);
            sprintf(msj, "contrasena %d", longitud);
        } else if (opcion == 3) {
            break;
        } else {
            printf("Opcion no valida.\n");
            continue;
        }

        // Enviar mensaje
        if (send(socket_desc, msj, strlen(msj), 0) < 0) {
            printf("Error al enviar mensaje.\n");
            return 1;
        }

        // Recibir respuesta del servidor
        if ((recv_size = recv(socket_desc, server_resp, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
            printf("Error al recibir datos.\n");
            return 1;
        }

        server_resp[recv_size] = '\0';
        printf("Respuesta del servidor: %s\n", server_resp);

        //system("pause");
        //system("cls");
    }

    // Cerrar el socket
    closesocket(socket_desc);
    WSACleanup();

    return 0;
}
