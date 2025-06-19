#include <iostream>
using namespace std;

// Estructura para cada miembro del árbol genealógico
struct Nodo {
    int id;
    string nombre;
    string fechaNacimiento;
    Nodo* izquierda;
    Nodo* derecha;
};

// Crear un nuevo nodo con datos
Nodo* crearNodo(int id, string nombre, string fecha) {
    Nodo* nuevo = new Nodo();
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->fechaNacimiento = fecha;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    return nuevo;
}

// Insertar un miembro en el árbol binario
Nodo* insertar(Nodo* raiz, int id, string nombre, string fecha) {
    if (raiz == NULL) {
        return crearNodo(id, nombre, fecha);
    }

    if (id < raiz->id) {
        raiz->izquierda = insertar(raiz->izquierda, id, nombre, fecha);
    } else {
        raiz->derecha = insertar(raiz->derecha, id, nombre, fecha);
    }

    return raiz;
}

// Recorrido inorden del árbol
void recorridoInorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoInorden(raiz->izquierda);
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre << " - Fecha: " << raiz->fechaNacimiento << endl;
        recorridoInorden(raiz->derecha);
    }
}

// Menú principal
int main() {
    Nodo* raiz = NULL;
    int opcion;
    int id;
    string nombre, fecha;

    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Insertar miembro" << endl;
        cout << "2. Mostrar recorrido inorden" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese ID: ";
                cin >> id;
                cin.ignore();
                cout << "Ingrese nombre: ";
                getline(cin, nombre);
                cout << "Ingrese fecha de nacimiento: ";
                getline(cin, fecha);
                raiz = insertar(raiz, id, nombre, fecha);
                break;

            case 2:
                cout << "\nRecorrido Inorden del árbol:\n";
                recorridoInorden(raiz);
                break;

            case 3:
                cout << "Saliendo del programa." << endl;
                break;

            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }

    } while (opcion != 3);

    return 0;
}


