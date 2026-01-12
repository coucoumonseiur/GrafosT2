#include <iostream>
#include <fstream>
using namespace std;

// Valor grande para infinito
const int INF = 999999;

// Nodo para lista de adyacencia
struct NodoAdyacente {
    int destino;
    int peso;
    NodoAdyacente* siguiente;
};

// Estructura para representar el grafo
struct Grafo {
    int vertices;
    NodoAdyacente** listaAdj;
};

// Crear un nuevo nodo para la lista de adyacencia
NodoAdyacente* crearNodo(int destino, int peso) {
    NodoAdyacente* nuevo = new NodoAdyacente;
    nuevo->destino = destino;
    nuevo->peso = peso;
    nuevo->siguiente = NULL;
    return nuevo;
}

// Inicializar un grafo con V vertices
void inicializarGrafo(Grafo* grafo, int v) {
    grafo->vertices = v;
    grafo->listaAdj = new NodoAdyacente*[v];
    for (int i = 0; i < v; i++) {
        grafo->listaAdj[i] = NULL;
    }
}

// Liberar memoria del grafo
void liberarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->vertices; i++) {
        NodoAdyacente* actual = grafo->listaAdj[i];
        while (actual != NULL) {
            NodoAdyacente* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
    delete[] grafo->listaAdj;
}

// Agregar arista al grafo (no dirigido)
void agregarArista(Grafo* grafo, int origen, int destino, int peso) {
    // Arista origen->destino
    NodoAdyacente* nuevo = crearNodo(destino, peso);
    nuevo->siguiente = grafo->listaAdj[origen];
    grafo->listaAdj[origen] = nuevo;
    
    // Arista destino->origen
    nuevo = crearNodo(origen, peso);
    nuevo->siguiente = grafo->listaAdj[destino];
    grafo->listaAdj[destino] = nuevo;
}

// Leer grafo desde archivo
Grafo leerDesdeArchivo(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo);
    Grafo grafo;
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << nombreArchivo << endl;
        inicializarGrafo(&grafo, 0);
        return grafo;
    }
    
    int numVertices;
    archivo >> numVertices;
    
    inicializarGrafo(&grafo, numVertices);
    
    int origen, destino, peso;
    while (archivo >> origen >> destino >> peso) {
        agregarArista(&grafo, origen, destino, peso);
    }
    
    archivo.close();
    return grafo;
}

// Mostrar el grafo
void imprimirGrafo(Grafo* grafo) {
    cout << "\nGrafo (Lista de Adyacencia):" << endl;
    for (int i = 0; i < grafo->vertices; i++) {
        cout << "Vertice " << i << ": ";
        NodoAdyacente* actual = grafo->listaAdj[i];
        while (actual != NULL) {
            cout << "-> " << actual->destino << " (Peso: " << actual->peso << ") ";
            actual = actual->siguiente;
        }
        cout << endl;
    }
}

// Algoritmo de Prim
void algoritmoPrim(Grafo* grafo) {
    bool* enArbol = new bool[grafo->vertices];
    int* clave = new int[grafo->vertices];
    int* padre = new int[grafo->vertices];
    
    for (int i = 0; i < grafo->vertices; i++) {
        enArbol[i] = false;
        clave[i] = INF;
        padre[i] = -1;
    }
    
    clave[0] = 0;
    
    for (int count = 0; count < grafo->vertices - 1; count++) {
        int u = -1;
        int minClave = INF;
        
        for (int v = 0; v < grafo->vertices; v++) {
            if (!enArbol[v] && clave[v] < minClave) {
                minClave = clave[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        enArbol[u] = true;
        
        NodoAdyacente* actual = grafo->listaAdj[u];
        while (actual != NULL) {
            int v = actual->destino;
            int peso = actual->peso;
            
            if (!enArbol[v] && peso < clave[v]) {
                clave[v] = peso;
                padre[v] = u;
            }
            actual = actual->siguiente;
        }
    }
    
    cout << "ARBOL MINIMO COBERTOR (Prim)" << endl;
    int pesoTotal = 0;
    for (int i = 1; i < grafo->vertices; i++) {
        if (padre[i] != -1) {
            cout << padre[i] << " - " << i << " (Peso: " << clave[i] << ")" << endl;
            pesoTotal += clave[i];
        }
    }
    cout << "Peso total del arbol: " << pesoTotal << endl;
    
    delete[] enArbol;
    delete[] clave;
    delete[] padre;
}

// Algoritmo de Dijkstra
void algoritmoDijkstra(Grafo* grafo, int origen, int destino) {
    int* distancia = new int[grafo->vertices];
    bool* visitado = new bool[grafo->vertices];
    int* anterior = new int[grafo->vertices];
    
    for (int i = 0; i < grafo->vertices; i++) {
        distancia[i] = INF;
        visitado[i] = false;
        anterior[i] = -1;
    }
    
    distancia[origen] = 0;
    
    for (int count = 0; count < grafo->vertices - 1; count++) {
        int u = -1;
        int minDist = INF;
        
        for (int v = 0; v < grafo->vertices; v++) {
            if (!visitado[v] && distancia[v] < minDist) {
                minDist = distancia[v];
                u = v;
            }
        }
        
        if (u == -1 || u == destino) break;
        
        visitado[u] = true;
        
        NodoAdyacente* actual = grafo->listaAdj[u];
        while (actual != NULL) {
            int v = actual->destino;
            int peso = actual->peso;
            
            if (!visitado[v] && distancia[u] != INF && 
                distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                anterior[v] = u;
            }
            actual = actual->siguiente;
        }
    }
    
    cout << "CAMINO MINIMO (Dijkstra)" << endl;
    cout << "Desde vertice " << origen << " hasta vertice " << destino << endl;
    
    if (distancia[destino] == INF) {
        cout << "No hay camino entre los vertices." << endl;
    } else {
        cout << "Distancia total: " << distancia[destino] << endl;
        
        int* camino = new int[grafo->vertices];
        int tamCamino = 0;
        int actual = destino;
        
        while (actual != -1) {
            camino[tamCamino] = actual;
            tamCamino++;
            actual = anterior[actual];
        }
        
        for (int i = 0; i < tamCamino / 2; i++) {
            int temp = camino[i];
            camino[i] = camino[tamCamino - 1 - i];
            camino[tamCamino - 1 - i] = temp;
        }
        
        cout << "Camino: ";
        for (int i = 0; i < tamCamino; i++) {
            cout << camino[i];
            if (i < tamCamino - 1) cout << " -> ";
        }
        cout << endl;
        
        delete[] camino;
    }
    
    delete[] distancia;
    delete[] visitado;
    delete[] anterior;
}

// Algoritmo de coloracion
void algoritmoColoracion(Grafo* grafo) {
    int* color = new int[grafo->vertices];
    bool* disponibles = new bool[grafo->vertices];
    
    for (int i = 0; i < grafo->vertices; i++) {
        color[i] = -1;
        disponibles[i] = true;
    }
    
    color[0] = 0;
    
    for (int u = 1; u < grafo->vertices; u++) {
        for (int i = 0; i < grafo->vertices; i++) {
            disponibles[i] = true;
        }
        
        NodoAdyacente* actual = grafo->listaAdj[u];
        while (actual != NULL) {
            int v = actual->destino;
            if (color[v] != -1) {
                disponibles[color[v]] = false;
            }
            actual = actual->siguiente;
        }
        
        int cr;
        for (cr = 0; cr < grafo->vertices; cr++) {
            if (disponibles[cr]) break;
        }
        
        color[u] = cr;
    }
    
    int maxColor = 0;
    for (int i = 0; i < grafo->vertices; i++) {
        if (color[i] > maxColor) {
            maxColor = color[i];
        }
    }
    int numCromatico = maxColor + 1;
    
    cout << "COLORACION DE GRAFOS" << endl;
    cout << "Numero cromatico: " << numCromatico << endl;
    cout << "Asignacion de colores por vertice:" << endl;
    
    for (int i = 0; i < grafo->vertices; i++) {
        cout << "Vertice " << i << " -> Color " << color[i] << endl;
    }
    
    bool valido = true;
    for (int u = 0; u < grafo->vertices && valido; u++) {
        NodoAdyacente* actual = grafo->listaAdj[u];
        while (actual != NULL && valido) {
            int v = actual->destino;
            if (color[u] == color[v]) {
                valido = false;
            }
            actual = actual->siguiente;
        }
    }
    
    if (valido) {
        cout << "La coloracion es valida." << endl;
    } else {
        cout << "ERROR: La coloracion no es valida." << endl;
    }
    
    delete[] color;
    delete[] disponibles;
}

// Programa principal
int main() {
    char nombreArchivo[100];
    cout << "Ingrese el nombre del archivo del grafo: ";
    cin >> nombreArchivo;
    
    Grafo grafo = leerDesdeArchivo(nombreArchivo);
    
    if (grafo.vertices == 0) {
        cout << "No se pudo cargar el grafo. Saliendo..." << endl;
        return 1;
    }
    
    cout << "Grafo cargado exitosamente" << endl;
    cout << "Numero de vertices: " << grafo.vertices << endl;
    
    int opcion = 0;
    bool salir = false;
    
    while (!salir) {
        cout << "MENU PRINCIPAL" << endl;
        cout << "1. Mostrar grafo" << endl;
        cout << "2. Algoritmo de Prim" << endl;
        cout << "3. Algoritmo de Dijkstra" << endl;
        cout << "4. Algoritmo de Coloracion" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        if (opcion == 1) {
            imprimirGrafo(&grafo);
        }
        else if (opcion == 2) {
            algoritmoPrim(&grafo);
        }
        else if (opcion == 3) {
            int origen, destino;
            cout << "Ingrese vertice origen: ";
            cin >> origen;
            cout << "Ingrese vertice destino: ";
            cin >> destino;
            
            if (origen < 0 || origen >= grafo.vertices || 
                destino < 0 || destino >= grafo.vertices) {
                cout << "Vertices invalidos. Deben estar entre 0 y " << grafo.vertices - 1 << endl;
            } else {
                algoritmoDijkstra(&grafo, origen, destino);
            }
        }
        else if (opcion == 4) {
            algoritmoColoracion(&grafo);
        }
        else if (opcion == 5) {
            cout << "Saliendo del programa..." << endl;
            salir = true;
        }
        else {
            cout << "Opcion invalida. Intente nuevamente." << endl;
        }
    }
    
    liberarGrafo(&grafo);
    
    return 0;
}