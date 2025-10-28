#include <iostream>
#include <string>
#include <stdexcept>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

using namespace std;


void algoritmo_vigenere( string entrada, 
                        string salida, 
                        string clave, 
                      bool opcion) 

{

    if (clave.empty()) {
        throw invalid_argument("La clave no puede estar vacía.");
    }

    int in = open(entrada.c_str(), O_RDONLY); // Archivo de entrada

    int out = open(salida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // Archivo de salida

    int BUFFER_SIZE = 4096;
    unsigned char buffer[BUFFER_SIZE]; // Creamos un buffer bien chimbita de 4096 bytes

    int indice = 0;
    int longitud = clave.length();
    
    int bytes;

    while ((bytes = read(in, buffer, BUFFER_SIZE)) > 0) { //Un While para que siga ejecutando siempre y cuando hayan bytes sin leer
        
        for (int i = 0; i < bytes; ++i) {
            
            unsigned char byte_clave = static_cast<unsigned char>(clave[indice]); // convierte el valor a un numero entre 0 y 255

            if (opcion) { // Encriptación
                buffer[i] = (buffer[i] + byte_clave) % 256; // Modulo 256 para codificar en binario (En el original eran 26 ya que coficaba mensajes de A-Z)
            } else { // Desemcriptación
                buffer[i] = (buffer[i] - byte_clave + 256) % 256;
            }

            indice = (indice + 1) % longitud;
        }

        int bytes_total = 0;
        while (bytes_total < bytes) { // Ciclo while que sirve para escribir la salida
            
            int bytes_escritos = write(out, buffer + bytes_total, bytes - bytes_total);
            
            bytes_total += bytes_escritos;
        }
    }


    close(in);
    close(out);


}

void mostrarMenu() {
    std::cout << "\n\n=== MENU PRINCIPAL ===";
    std::cout << "\n0. Encriptar";
    std::cout << "\n1. Desencriptar";
    std::cout << "\n2. Salir del programa";
    std::cout << "\nSeleccione una opcion: ";
}

int main(int argc, char *argv[])
{

    if (argc < 3) {
        cerr << "Error: Faltan argumentos." << endl;
        cerr << "Uso: " << argv[0] << " <archivo_entrada> <archivo_salida>" << endl;
        return 1;
    }

    const string clave = "MaxiBoliviano123!";
    int opcion;

    do {
        mostrarMenu();

        cin >> opcion;


        try {
            switch(opcion){
                case 0: {

                    cout<< "Encriptando...\n";

                    algoritmo_vigenere(argv[1], argv[2], clave, true);

                    break;

                }

                case 1: {

                    cout<< "Desencriptando...\n";

                    algoritmo_vigenere(argv[1], argv[2], clave, false);
                    break;

                }

                case 2: {

                    cout << "\nSaliendo...\n";
                    break;

                }

                default: {

                    cout << "\nOpcion no valida, intenta de nuevo.\n";
                    break;

                }    
            }
        } catch (exception& e) {
            cerr << "\n*** ERROR: " << e.what() << " ***\n";
        }
    } while(opcion != 2);

    return 0;
}