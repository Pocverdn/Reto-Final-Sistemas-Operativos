#include <iostream>
#include <string>
#include "encriptacion.cpp"
#include "compresion.cpp"
using namespace std;

void mostrarMenu() {
    std::cout << "\n\n=== MENU PRINCIPAL ===";
    std::cout << "\n0. Encriptar";
    std::cout << "\n1. Desencriptar";
    std::cout << "\n2. Comprimir";
    std::cout << "\n3. Descomprimir";
    std::cout << "\n4. Encriptar y Comprimir";
    std::cout << "\n5. Desencriptar y Descomprimir";
    std::cout << "\n6. Salir del programa";
    std::cout << "\nSeleccione una opcion: ";
}

int main(int argc, char *argv[])
{
    int opcion;
    const string clave = "ABC";

    do {
        mostrarMenu();
        cin >> opcion;

        switch(opcion){

            // ------------------------------
            // VIGENERE ENCODE
            // ------------------------------
            case 0: {
                string nombreArchivo;
                string salidaArchivo;

                cout << "Archivo para encriptar: ";
                cin >> nombreArchivo;
                cout << "Nombre de salida: ";
                cin >> salidaArchivo;

                cout << "Encriptando...\n";
                algoritmo_vigenere(nombreArchivo, salidaArchivo, clave, true);
                break;
            }

            case 1: {
                string nombreArchivo;
                string salidaArchivo;

                cout << "Archivo para desencriptar: ";
                cin >> nombreArchivo;
                cout << "Nombre de salida: ";
                cin >> salidaArchivo;

                cout << "Desencriptando...\n";
                algoritmo_vigenere(nombreArchivo, salidaArchivo, clave, false);
                break;
            }

            case 2: {
                string input;
                string encoded;
                string tree;

                cout << "Archivo para comprimir: ";
                cin >> input;

                cout << "Archivo comprimido (.huff): ";
                cin >> encoded;

                cout << "Archivo del arbol (.tree): ";
                cin >> tree;

                encodeHuffman(input, encoded, tree);
                break;
            }

            case 3: {
                string encoded;
                string tree;
                string output;

                cout << "Archivo comprimido (.huff): ";
                cin >> encoded;

                cout << "Archivo del arbol (.tree): ";
                cin >> tree;

                cout << "Archivo de salida: ";
                cin >> output;

                decodeHuffman(encoded, tree, output);
                break;
            }

            case 4: {
                string archivoEntrada;
                string archivoEncriptado;
                string archivoComprimido;
                string archivoArbol;
                        
                cout << "Archivo para encriptar y comprimir: ";
                cin >> archivoEntrada;
                        
                cout << "Archivo encriptado: ";
                cin >> archivoEncriptado;
                        
                cout << "Archivo comprimido (.huff): ";
                cin >> archivoComprimido;
                        
                cout << "Archivo del arbol (.tree): ";
                cin >> archivoArbol;
                        
                cout << "Encriptando...\n";
                algoritmo_vigenere(archivoEntrada, archivoEncriptado, clave, true);
                        
                cout << "Comprimiendo...\n";
                encodeHuffman(archivoEncriptado, archivoComprimido, archivoArbol);
                        
                cout << "Proceso completado.\n";
                break;
            }


            case 5: {
                string archivoComprimido;
                string archivoArbol;
                string archivoDescomprimido;
                string archivoFinal;
                        
                cout << "Archivo comprimido (.huff): ";
                cin >> archivoComprimido;
                        
                cout << "Archivo del arbol (.tree): ";
                cin >> archivoArbol;
                        
                cout << "Archivo descomprimido temporal: ";
                cin >> archivoDescomprimido;
                        
                cout << "Archivo final desencriptado: ";
                cin >> archivoFinal;
                        
                cout << "Descomprimiendo...\n";
                decodeHuffman(archivoComprimido, archivoArbol, archivoDescomprimido);
                        
                cout << "Desencriptando...\n";
                algoritmo_vigenere(archivoDescomprimido, archivoFinal, clave, false);
                        
                cout << "Proceso completado.\n";
                break;
            }


            case 6:
                cout << "\nSaliendo...\n";
                return 0;

            default:
                cout << "\nOpción no válida, intenta de nuevo.\n";
                break;
        }

    } while(opcion != 6);

    return 0;
}
