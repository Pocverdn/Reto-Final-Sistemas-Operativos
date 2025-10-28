#include <iostream>
#include <string>

using namespace std;

void mostrarMenu() {
    std::cout << "\n\n=== MENU PRINCIPAL ===";
    std::cout << "\n0. Metodo de prueba";
    std::cout << "\n1. Salir del programa";
    std::cout << "\nSeleccione una opcion: ";
}

int main(int argc, char *argv[])
{
    int opcion;

    do {
        mostrarMenu();

        cin >> opcion;

        switch(opcion){
            case 0: {

                cout << argv[1];
                break;

            }

            case 1: {

                cout << "\nSaliendo...\n";
                break;

            }

            default: {

                cout << "\nOpcion no valida, intenta de nuevo.\n";
                break;

            }    
        }
    } while(opcion != 1);

    return 0;
}