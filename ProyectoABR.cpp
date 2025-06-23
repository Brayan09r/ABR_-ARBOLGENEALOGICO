#include <iostream>
#include <string>
#include <limits>
#include <fstream>
using namespace std;

// ------------------------- ESTRUCTURA DEL NODO -------------------------
// Representa a cada miembro de la familia en el �rbol geneal�gico
struct Nodo {
    int id;             // Identificador �nico del miembro
    string nombre;      // Nombre del miembro
    int edad;           // Edad del miembro
    Nodo* izquierda;    // Puntero al hijo izquierdo (primer hijo)
    Nodo* derecha;      // Puntero al hijo derecho (segundo hijo)
    Nodo* padre;        // Puntero al padre del nodo (para rastrear ascendencia)
};

// ------------------------- FUNCI�N PARA CREAR UN NODO -------------------------
// Crea un nuevo nodo con los datos proporcionados y lo inicializa
Nodo* crearNodo(int id, string nombre, int edad, Nodo* padre = NULL) {
    Nodo* nuevo = new Nodo();  // Reserva memoria para el nuevo nodo
    nuevo->id = id;            // Asigna el ID
    nuevo->nombre = nombre;    // Asigna el nombre
    nuevo->edad = edad;        // Asigna la edad
    nuevo->izquierda = NULL;   // Inicializa hijo izquierdo como NULL
    nuevo->derecha = NULL;     // Inicializa hijo derecho como NULL
    nuevo->padre = padre;      // Asigna el padre (si se proporciona)
    return nuevo;              // Retorna el nuevo nodo creado
}

// ------------------------- FUNCI�N PARA BUSCAR UN NODO POR ID -------------------------
// Busca recursivamente un nodo en el �rbol dado un ID
Nodo* buscarNodo(Nodo* raiz, int id) {
    // Caso base: Si el �rbol est� vac�o o se encontr� el nodo
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    // Busca en el sub�rbol izquierdo
    Nodo* encontrado = buscarNodo(raiz->izquierda, id);
    // Si no se encontr� en el izquierdo, busca en el derecho
    if (encontrado == NULL) {
        encontrado = buscarNodo(raiz->derecha, id);
    }
    return encontrado;  // Retorna el nodo encontrado (o NULL si no existe)
}

// ------------------------- FUNCI�N PARA VERIFICAR SI UN ID EXISTE -------------------------
// Retorna `true` si el ID ya est� en uso en el �rbol
bool existeID(Nodo* raiz, int id) {
    return buscarNodo(raiz, id) != NULL;  // Usa `buscarNodo` para verificar existencia
}

// ------------------------- FUNCI�N PARA INSERTAR UN NUEVO HIJO -------------------------
// Agrega un nuevo miembro como hijo izquierdo o derecho de un padre existente
bool insertarHijo(Nodo* raiz, int idPadre, int id, string nombre, int edad) {
    // Verifica si el ID ya existe
    if (existeID(raiz, id)) {
        cout << "Error: El ID " << id << " ya existe en el �rbol." << endl;
        return false;
    }

    // Busca al padre por su ID
    Nodo* padre = buscarNodo(raiz, idPadre);
    if (padre == NULL) {
        cout << "Error: No se encontr� el padre con ID " << idPadre << endl;
        return false;
    }

    // Crea el nuevo nodo con referencia al padre
    Nodo* nuevo = crearNodo(id, nombre, edad, padre);

    // Intenta asignar como hijo izquierdo (si est� disponible)
    if (padre->izquierda == NULL) {
        padre->izquierda = nuevo;
        cout << nombre << " agregado como hijo izquierdo de " << padre->nombre << endl;
        return true;
    } 
    // Si el izquierdo est� ocupado, intenta como hijo derecho
    else if (padre->derecha == NULL) {
        padre->derecha = nuevo;
        cout << nombre << " agregado como hijo derecho de " << padre->nombre << endl;
        return true;
    } 
    // Si ambos hijos est�n ocupados, muestra error y libera memoria
    else {
        cout << "Error: " << padre->nombre << " ya tiene dos hijos." << endl;
        delete nuevo;
        return false;
    }
}

// ------------------------- RECORRIDO INORDEN (Izquierda - Ra�z - Derecha) -------------------------
// Muestra los nodos en orden: sub�rbol izquierdo, ra�z, sub�rbol derecho
void recorridoInorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoInorden(raiz->izquierda);  // Recorre izquierda primero
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime ra�z
        recorridoInorden(raiz->derecha);    // Recorre derecha despu�s
    }
}

// ------------------------- RECORRIDO PREORDEN (Ra�z - Izquierda - Derecha) -------------------------
// Muestra los nodos en orden: ra�z, sub�rbol izquierdo, sub�rbol derecho
void recorridoPreorden(Nodo* raiz) {
    if (raiz != NULL) {
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime ra�z primero
        recorridoPreorden(raiz->izquierda);  // Luego izquierda
        recorridoPreorden(raiz->derecha);    // Finalmente derecha
    }
}

// ------------------------- RECORRIDO POSTORDEN (Izquierda - Derecha - Ra�z) -------------------------
// Muestra los nodos en orden: sub�rbol izquierdo, sub�rbol derecho, ra�z
void recorridoPostorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoPostorden(raiz->izquierda);  // Recorre izquierda
        recorridoPostorden(raiz->derecha);    // Recorre derecha
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime ra�z al final
    }
}

// ------------------------- MOSTRAR ASCENDENCIA DE UN NODO -------------------------
// Muestra la l�nea de ancestros desde un nodo hasta la ra�z
void mostrarAscendencia(Nodo* nodo) {
    if (nodo == NULL) return;  // Si el nodo es NULL, termina
    
    cout << "\nAscendencia de " << nodo->nombre << ":" << endl;
    cout << nodo->nombre;  // Empieza con el nombre del nodo actual
    
    // Recorre hacia arriba (padres) hasta llegar a la ra�z
    while (nodo->padre != NULL) {
        nodo = nodo->padre;  // Avanza al padre
        cout << " <- " << nodo->nombre;  // Muestra el nombre del padre
    }
    cout << "\n" << endl;  // Salto de l�nea al final
}

// ------------------------- LIBERAR MEMORIA DEL �RBOL -------------------------
// Elimina todos los nodos del �rbol para evitar fugas de memoria
void liberarArbol(Nodo* raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);  // Libera sub�rbol izquierdo
        liberarArbol(raiz->derecha);    // Libera sub�rbol derecho
        delete raiz;                    // Libera el nodo actual
    }
}

// ------------------------- FUNCI�N PARA INGRESAR UN NOMBRE V�LIDO -------------------------
// Solicita y valida que el nombre no est� vac�o
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

// ------------------------- FUNCI�N PARA INGRESAR UNA EDAD V�LIDA -------------------------
// Valida que la edad est� entre 1 y 120 a�os
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


// ------------------------- FUNCI�N PARA INGRESAR UN ID V�LIDO -------------------------
// Valida que el ID sea positivo y (opcionalmente) que exista o no en el �rbol
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

// ========================= PROGRAMA PRINCIPAL =========================
int main() {
    setlocale(LC_CTYPE, "Spanish");  // Configuraci�n regional para caracteres en espa�ol
    Nodo* raiz = NULL;  // Inicializa el �rbol vac�o

    int opcion;
    do {
        // Men� principal
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

        // Manejo de opciones
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

    liberarArbol(raiz);  // Libera toda la memoria del �rbol antes de salir
    return 0;
}
