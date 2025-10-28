#include <iostream>
#include <string>
#include <stdexcept>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

using namespace std;


void algoritmo_vigenere(const string& entrada, 
                      const string& salida, 
                      const string& clave, 
                      bool opcion) 

{

    if (clave.empty()) {
        throw invalid_argument("La clave no puede estar vacía.");
    }

    int in = open(entrada.c_str(), O_RDONLY);
    if (in < 0) {
        throw runtime_error("Error abriendo entrada: " + string(strerror(errno)));
    }

    int out = open(salida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        close(in);
        throw runtime_error("Error abriendo salida: " + string(strerror(errno)));
    }

    const size_t BUFFER_SIZE = 4096;

    unsigned char buffer[BUFFER_SIZE];

    ssize_t indice = 0;
    const ssize_t longitud = clave.length();
    
    ssize_t bytes;

    while ((bytes = read(in, buffer, BUFFER_SIZE)) > 0) {
        
        for (ssize_t i = 0; i < bytes; ++i) {
            
            unsigned char byte_clave = static_cast<unsigned char>(clave[indice]);

            if (opcion) {
                buffer[i] = (buffer[i] + byte_clave) % 256; // Modulo 255 para codificar en binario (En el original eran 26)
            } else {
                buffer[i] = (buffer[i] - byte_clave + 256) % 256;
            }

            indice = (indice + 1) % longitud;
        }

        ssize_t bytes_total = 0;
        while (bytes_total < bytes) {
            ssize_t bytes_escritos = write(out, 
                                           buffer + bytes_total, 
                                           bytes - bytes_total);
            
            if (bytes_escritos < 0) {
                close(in);
                close(out);
                throw runtime_error("Error escribiendo en salida: " + string(strerror(errno)));
            }
            bytes_total += bytes_escritos;
        }
    }

    if (bytes < 0) {
        close(in);
        close(out);
        throw runtime_error("Error leyendo de entrada: " + string(strerror(errno)));
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

    const string clave = "MiClaveSuperSegura123!";
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
        } catch (const std::exception& e) {
            cerr << "\n*** ERROR: " << e.what() << " ***\n";
        }
    } while(opcion != 2);

    return 0;
}