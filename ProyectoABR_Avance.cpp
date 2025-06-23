#include <iostream>
#include <string>
#include <limits>
#include <fstream>
using namespace std;

// Estructura para cada miembro del �rbol geneal�gico
struct Nodo {
    int id;
    string nombre;
    int edad;
    Nodo* izquierda;
    Nodo* derecha;
    Nodo* padre;
};

// Crear un nuevo nodo con datos
Nodo* crearNodo(int id, string nombre, int edad, Nodo* padre = NULL) {
    Nodo* nuevo = new Nodo();
    nuevo->id = id;
    nuevo->nombre = nombre;
    nuevo->edad = edad;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    nuevo->padre = padre;
    return nuevo;
}

// Buscar un nodo por ID
Nodo* buscarNodo(Nodo* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    Nodo* encontrado = buscarNodo(raiz->izquierda, id);
    if (encontrado == NULL) {
        encontrado = buscarNodo(raiz->derecha, id);
    }
    return encontrado;
}

// Verificar si un ID ya existe en el �rbol
bool existeID(Nodo* raiz, int id) {
    return buscarNodo(raiz, id) != NULL;
}

// Insertar un nuevo miembro como hijo izquierdo o derecho seg�n disponibilidad
bool insertarHijo(Nodo* raiz, int idPadre, int id, string nombre, int edad) {
    if (existeID(raiz, id)) {
        cout << "Error: El ID " << id << " ya existe en el �rbol." << endl;
        return false;
    }

    Nodo* padre = buscarNodo(raiz, idPadre);
    if (padre == NULL) {
        cout << "Error: No se encontr� el padre con ID " << idPadre << endl;
        return false;
    }

    Nodo* nuevo = crearNodo(id, nombre, edad, padre);

    if (padre->izquierda == NULL) {
        padre->izquierda = nuevo;
        cout << nombre << " agregado como hijo izquierdo de " << padre->nombre << endl;
        return true;
    } else if (padre->derecha == NULL) {
        padre->derecha = nuevo;
        cout << nombre << " agregado como hijo derecho de " << padre->nombre << endl;
        return true;
    } else {
        cout << "Error: " << padre->nombre << " ya tiene dos hijos." << endl;
        delete nuevo;
        return false;
    }
}

// Recorrido inorden del �rbol (Izquierdo-Ra�z-Derecho)
void recorridoInorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoInorden(raiz->izquierda);
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;
        recorridoInorden(raiz->derecha);
    }
}

// Recorrido preorden del �rbol (Ra�z-Izquierdo-Derecho)
void recorridoPreorden(Nodo* raiz) {
    if (raiz != NULL) {
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;
        recorridoPreorden(raiz->izquierda);
        recorridoPreorden(raiz->derecha);
    }
}

// Recorrido postorden del �rbol (Izquierdo-Derecho-Ra�z)
void recorridoPostorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoPostorden(raiz->izquierda);
        recorridoPostorden(raiz->derecha);
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;
    }
}

// Mostrar la l�nea de ascendencia de un nodo
void mostrarAscendencia(Nodo* nodo) {
    if (nodo == NULL) return;
    
    cout << "\nAscendencia de " << nodo->nombre << ":" << endl;
    cout << nodo->nombre;
    
    while (nodo->padre != NULL) {
        nodo = nodo->padre;
        cout << " <- " << nodo->nombre;
    }
    cout << "\n" << endl;
}

// Funci�n para liberar memoria del �rbol
void liberarArbol(Nodo* raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        delete raiz;
    }
}

// Funci�n para ingresar un nombre v�lido
string ingresarNombre() {
    string nombre;
    cout << "Ingrese nombre: ";
    do {
        getline(cin, nombre);
        
        if (nombre.empty()) {
            cout << "Error: El nombre no puede estar vac�o. Intente nuevamente: ";
        }
    } while (nombre.empty());
    
    return nombre;
}

// Funci�n para ingresar una edad v�lida
int ingresarEdad() {
    int edad;
    while (true) {
        cout << "Ingrese edad en anos: ";
        cin >> edad;
        
        if (cin.fail() || edad <= 0 || edad > 120) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Edad inv�lida. Debe ser un n�mero entre 1 y 120.\n";
        } else {
            cin.ignore();
            return edad;
        }
    }
}

// Funci�n para ingresar un ID v�lido
int ingresarID(Nodo* raiz, const string& mensaje, bool debeExistir = false) {
    int id;
    while(true) {
        cout << mensaje;
        cin >> id;
        if(cin.fail() || id <= 0) {
            cout << "ID inv�lido. Debe ser un n�mero positivo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        bool existe = existeID(raiz, id);
        if((debeExistir && !existe) || (!debeExistir && existe)) {
            cout << "Error: El ID " << id << (debeExistir ? " no existe" : " ya existe") << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }
    return id;
}

int main() {
    setlocale(LC_CTYPE, "Spanish"); 
    Nodo* raiz = NULL;

    int opcion;
    do {
        cout << "\n--- �RBOL GENEAL�GICO ---" << endl;
        cout << "1. Insertar miembro ra�z" << endl;
        cout << "2. Insertar nuevo miembro" << endl;
        cout << "3. Mostrar recorrido Inorden" << endl;
        cout << "4. Mostrar recorrido Preorden" << endl;
        cout << "5. Mostrar recorrido Postorden" << endl;
        cout << "6. Mostrar ascendencia de un miembro" << endl;
        cout << "7. Salir" << endl;
        cout << "Seleccione una opci�n: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 1) {
            if (raiz == NULL) {
                int id = ingresarID(raiz, "Ingrese ID del ancestro m�s antiguo: ");
                string nombre = ingresarNombre();
                int edad = ingresarEdad();
                
                raiz = crearNodo(id, nombre, edad);
                cout << "\nMiembro ra�z '" << nombre << "' agregado con �xito.\n";
            } else {
                cout << "Error: Ya existe una ra�z. Solo se permite una." << endl;
            }
        }
        else if (opcion == 2) {
            if (raiz == NULL) {
                cout << "Error: Primero debe crear el miembro ra�z." << endl;
                continue;
            }
            
            int idPadre = ingresarID(raiz, "Ingrese ID del padre: ", true);
            int id = ingresarID(raiz, "Ingrese ID del nuevo miembro: ");
            
            if (id == idPadre) {
                cout << "Error: Un miembro no puede ser padre de s� mismo." << endl;
                continue;
            }
            
            string nombre = ingresarNombre();
            int edad = ingresarEdad();
            
            insertarHijo(raiz, idPadre, id, nombre, edad);
        }
        else if (opcion == 3) {
            if (raiz == NULL) {
                cout << "El �rbol est� vac�o." << endl;
            } else {
                cout << "\nRecorrido Inorden del �rbol geneal�gico:\n";
                recorridoInorden(raiz);
            }
        }
        else if (opcion == 4) {
            if (raiz == NULL) {
                cout << "El �rbol est� vac�o." << endl;
            } else {
                cout << "\nRecorrido Preorden del �rbol geneal�gico:\n";
                recorridoPreorden(raiz);
            }
        }
        else if (opcion == 5) {
            if (raiz == NULL) {
                cout << "El �rbol est� vac�o." << endl;
            } else {
                cout << "\nRecorrido Postorden del �rbol geneal�gico:\n";
                recorridoPostorden(raiz);
            }
        }
        else if (opcion == 6) {
            if (raiz == NULL) {
                cout << "El �rbol est� vac�o." << endl;
                continue;
            }
            
            int idConsulta = ingresarID(raiz, "Ingrese ID del miembro para ver su ascendencia: ", true);
            Nodo* miembro = buscarNodo(raiz, idConsulta);
            mostrarAscendencia(miembro);
        }
        else if (opcion == 7) {
            cout << "Saliendo del programa..." << endl;
        }
        else {
            cout << "Opci�n inv�lida. Intente nuevamente." << endl;
        }

    } while (opcion != 7);

    liberarArbol(raiz);
    return 0;
}
