#include <iostream>
#include <string>
#include <limits>
#include <fstream>
using namespace std;

// ------------------------- ESTRUCTURA DEL NODO -------------------------
// Representa a cada miembro de la familia en el árbol genealógico
struct Nodo {
    int id;             // Identificador único del miembro
    string nombre;      // Nombre del miembro
    int edad;           // Edad del miembro
    Nodo* izquierda;    // Puntero al hijo izquierdo (primer hijo)
    Nodo* derecha;      // Puntero al hijo derecho (segundo hijo)
    Nodo* padre;        // Puntero al padre del nodo (para rastrear ascendencia)
};

// ------------------------- FUNCIÓN PARA CREAR UN NODO -------------------------
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

// ------------------------- FUNCIÓN PARA BUSCAR UN NODO POR ID -------------------------
// Busca recursivamente un nodo en el árbol dado un ID
Nodo* buscarNodo(Nodo* raiz, int id) {
    // Caso base: Si el árbol está vacío o se encontró el nodo
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    // Busca en el subárbol izquierdo
    Nodo* encontrado = buscarNodo(raiz->izquierda, id);
    // Si no se encontró en el izquierdo, busca en el derecho
    if (encontrado == NULL) {
        encontrado = buscarNodo(raiz->derecha, id);
    }
    return encontrado;  // Retorna el nodo encontrado (o NULL si no existe)
}

// ------------------------- FUNCIÓN PARA VERIFICAR SI UN ID EXISTE -------------------------
// Retorna `true` si el ID ya está en uso en el árbol
bool existeID(Nodo* raiz, int id) {
    return buscarNodo(raiz, id) != NULL;  // Usa `buscarNodo` para verificar existencia
}

// ------------------------- FUNCIÓN PARA INSERTAR UN NUEVO HIJO -------------------------
// Agrega un nuevo miembro como hijo izquierdo o derecho de un padre existente
bool insertarHijo(Nodo* raiz, int idPadre, int id, string nombre, int edad) {
    // Verifica si el ID ya existe
    if (existeID(raiz, id)) {
        cout << "Error: El ID " << id << " ya existe en el árbol." << endl;
        return false;
    }

    // Busca al padre por su ID
    Nodo* padre = buscarNodo(raiz, idPadre);
    if (padre == NULL) {
        cout << "Error: No se encontró el padre con ID " << idPadre << endl;
        return false;
    }

    // Crea el nuevo nodo con referencia al padre
    Nodo* nuevo = crearNodo(id, nombre, edad, padre);

    // Intenta asignar como hijo izquierdo (si está disponible)
    if (padre->izquierda == NULL) {
        padre->izquierda = nuevo;
        cout << nombre << " agregado como hijo izquierdo de " << padre->nombre << endl;
        return true;
    } 
    // Si el izquierdo está ocupado, intenta como hijo derecho
    else if (padre->derecha == NULL) {
        padre->derecha = nuevo;
        cout << nombre << " agregado como hijo derecho de " << padre->nombre << endl;
        return true;
    } 
    // Si ambos hijos están ocupados, muestra error y libera memoria
    else {
        cout << "Error: " << padre->nombre << " ya tiene dos hijos." << endl;
        delete nuevo;
        return false;
    }
}

// ------------------------- RECORRIDO INORDEN (Izquierda - Raíz - Derecha) -------------------------
// Muestra los nodos en orden: subárbol izquierdo, raíz, subárbol derecho
void recorridoInorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoInorden(raiz->izquierda);  // Recorre izquierda primero
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime raíz
        recorridoInorden(raiz->derecha);    // Recorre derecha después
    }
}

// ------------------------- RECORRIDO PREORDEN (Raíz - Izquierda - Derecha) -------------------------
// Muestra los nodos en orden: raíz, subárbol izquierdo, subárbol derecho
void recorridoPreorden(Nodo* raiz) {
    if (raiz != NULL) {
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime raíz primero
        recorridoPreorden(raiz->izquierda);  // Luego izquierda
        recorridoPreorden(raiz->derecha);    // Finalmente derecha
    }
}

// ------------------------- RECORRIDO POSTORDEN (Izquierda - Derecha - Raíz) -------------------------
// Muestra los nodos en orden: subárbol izquierdo, subárbol derecho, raíz
void recorridoPostorden(Nodo* raiz) {
    if (raiz != NULL) {
        recorridoPostorden(raiz->izquierda);  // Recorre izquierda
        recorridoPostorden(raiz->derecha);    // Recorre derecha
        cout << "ID: " << raiz->id << " - Nombre: " << raiz->nombre 
             << " - Edad: " << raiz->edad << " anos" << endl;  // Imprime raíz al final
    }
}

// ------------------------- MOSTRAR ASCENDENCIA DE UN NODO -------------------------
// Muestra la línea de ancestros desde un nodo hasta la raíz
void mostrarAscendencia(Nodo* nodo) {
    if (nodo == NULL) return;  // Si el nodo es NULL, termina
    
    cout << "\nAscendencia de " << nodo->nombre << ":" << endl;
    cout << nodo->nombre;  // Empieza con el nombre del nodo actual
    
    // Recorre hacia arriba (padres) hasta llegar a la raíz
    while (nodo->padre != NULL) {
        nodo = nodo->padre;  // Avanza al padre
        cout << " <- " << nodo->nombre;  // Muestra el nombre del padre
    }
    cout << "\n" << endl;  // Salto de línea al final
}

// ------------------------- LIBERAR MEMORIA DEL ÁRBOL -------------------------
// Elimina todos los nodos del árbol para evitar fugas de memoria
void liberarArbol(Nodo* raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);  // Libera subárbol izquierdo
        liberarArbol(raiz->derecha);    // Libera subárbol derecho
        delete raiz;                    // Libera el nodo actual
    }
}

// ------------------------- FUNCIÓN PARA INGRESAR UN NOMBRE VÁLIDO -------------------------
// Solicita y valida que el nombre no esté vacío
string ingresarNombre() {
    string nombre;
    cout << "Ingrese nombre: ";
    do {
        getline(cin, nombre);
        if (nombre.empty()) {
            cout << "Error: El nombre no puede estar vacío. Intente nuevamente: ";
        }
    } while (nombre.empty());
    return nombre;
}

// ------------------------- FUNCIÓN PARA INGRESAR UNA EDAD VÁLIDA -------------------------
// Valida que la edad esté entre 1 y 120 años
int ingresarEdad() {
    int edad;
    while (true) {
        cout << "Ingrese edad en anos: ";
        cin >> edad;
        if (cin.fail() || edad <= 0 || edad > 120) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Edad inválida. Debe ser un número entre 1 y 120.\n";
        } else {
            cin.ignore();
            return edad;
        }
    }
}


// ------------------------- FUNCIÓN PARA INGRESAR UN ID VÁLIDO -------------------------
// Valida que el ID sea positivo y (opcionalmente) que exista o no en el árbol
int ingresarID(Nodo* raiz, const string& mensaje, bool debeExistir = false) {
    int id;
    while(true) {
        cout << mensaje;
        cin >> id;
        if(cin.fail() || id <= 0) {
            cout << "ID inválido. Debe ser un número positivo.\n";
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
    setlocale(LC_CTYPE, "Spanish");  // Configuración regional para caracteres en español
    Nodo* raiz = NULL;  // Inicializa el árbol vacío

    int opcion;
    do {
        // Menú principal
        cout << "\n--- ÁRBOL GENEALÓGICO ---" << endl;
        cout << "1. Insertar miembro raíz" << endl;
        cout << "2. Insertar nuevo miembro" << endl;
        cout << "3. Mostrar recorrido Inorden" << endl;
        cout << "4. Mostrar recorrido Preorden" << endl;
        cout << "5. Mostrar recorrido Postorden" << endl;
        cout << "6. Mostrar ascendencia de un miembro" << endl;
        cout << "7. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Manejo de opciones
        if (opcion == 1) {
            if (raiz == NULL) {
                int id = ingresarID(raiz, "Ingrese ID del ancestro más antiguo: ");
                string nombre = ingresarNombre();
                int edad = ingresarEdad();
                raiz = crearNodo(id, nombre, edad);
                cout << "\nMiembro raíz '" << nombre << "' agregado con éxito.\n";
            } else {
                cout << "Error: Ya existe una raíz. Solo se permite una." << endl;
            }
        }
        else if (opcion == 2) {
            if (raiz == NULL) {
                cout << "Error: Primero debe crear el miembro raíz." << endl;
                continue;
            }
            int idPadre = ingresarID(raiz, "Ingrese ID del padre: ", true);
            int id = ingresarID(raiz, "Ingrese ID del nuevo miembro: ");
            if (id == idPadre) {
                cout << "Error: Un miembro no puede ser padre de sí mismo." << endl;
                continue;
            }
            string nombre = ingresarNombre();
            int edad = ingresarEdad();
            insertarHijo(raiz, idPadre, id, nombre, edad);
        }
        else if (opcion == 3) {
            if (raiz == NULL) {
                cout << "El árbol está vacío." << endl;
            } else {
                cout << "\nRecorrido Inorden del árbol genealógico:\n";
                recorridoInorden(raiz);
            }
        }
        else if (opcion == 4) {
            if (raiz == NULL) {
                cout << "El árbol está vacío." << endl;
            } else {
                cout << "\nRecorrido Preorden del árbol genealógico:\n";
                recorridoPreorden(raiz);
            }
        }
        else if (opcion == 5) {
            if (raiz == NULL) {
                cout << "El árbol está vacío." << endl;
            } else {
                cout << "\nRecorrido Postorden del árbol genealógico:\n";
                recorridoPostorden(raiz);
            }
        }
        else if (opcion == 6) {
            if (raiz == NULL) {
                cout << "El árbol está vacío." << endl;
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
            cout << "Opción inválida. Intente nuevamente." << endl;
        }

    } while (opcion != 7);

    liberarArbol(raiz);  // Libera toda la memoria del árbol antes de salir
    return 0;
}
