import socket

SERVER_IP = "127.0.0.1"
SERVERPORT = 5000
BUFFER_SIZE = 1024

def main():
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.connect((SERVER_IP, SERVERPORT))

    try:
        while True:
            print("\nOpciones:")
            print("1. Generar nombre de usuario")
            print("2. Generar contraseña")
            print("3. Salir")
            opcion = input("Elija una opción: ")

            if opcion == '1':
                longitud = input("Ingrese la longitud del nombre entre 5 y 15: ")
                mensaje = f"nombre {longitud}"
            elif opcion == '2':
                longitud = input("Ingrese la longitud de la contraseña entre 8 y 50: ")
                mensaje = f"contrasena {longitud}"
            elif opcion == '3':
                print("Salir.")
                break
            else:
                print("Opción no válida.")
                continue

            clientSocket.send(mensaje.encode())

            respuesta = clientSocket.recv(BUFFER_SIZE).decode()
            print(f"Respuesta: {respuesta}")

    except Exception as e:
            print(f"Error: {e}")

    finally:
        clientSocket.close()
if __name__ == "__main__":
    main()
