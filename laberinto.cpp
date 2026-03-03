#include <iostream> // Esto permite que ingresen y salgan los datos
#include <vector> // Esto permite utilizar los vectores dinamicos o "matrices"
#include <stack> // Se utiliza en el algoritmo de generacion (backtracking/dfs)
#include <queue> // Se utiliza en el algoritmo de resolucion del laberinto (bfs) "Primero en entrar primero en salir "
#include <ctime> // Permite acceder al relog del sistema 
#include <cstdlib> // Contiene la funciones de aletoriedad lo utizo para generar numeros aleatorios

using namespace std;
// Estructura para manejar las coordenadas (Y, X)
struct Punto {
    int y, x;
};
// Variables constantes 
const int PARED = 1;
const int CAMINO = 0;
const int RUTA_SOLUCION = 2;
// Direcciones de movimiento (salta de 2 en 2 para generar)
// {y,x}
int dy[] = {0,0,2,-2};
int dx[] = {2,-2,0,0};
// Aqui creo la matriz del labertinto "Aqui tambien le socilicito la memoria a utilizar"
int** crearMatriz(int filas, int columnas) {
    // Aqui designo el espacio para las filas ("Arreglo de punteros")
    int** matriz = new int *[filas];
// En cada fila, se hace espacio para las columnas
for (int i = 0; i < filas; i++) {
    matriz[i] = new int[columnas];
    // Se llena de una vez todo con PAREDES
    for (int j = 0; j < columnas; j++) {
        matriz[i][j]= PARED;
    }
}
return matriz;
}
void destruirMatriz(int** matriz, int filas) {   // Aqui le indico que destruya ese proceso para volver la liberar la memoria que le solicite al princicio
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i]; // Aqui borra cada fila 
    }
    delete[] matriz; // Aqui borra el contenedor de filas 
}
void imprimirLaberinto(int** matriz, int filas, int columnas){   // Aqui en esta funcion (void) se imprime mi tablero 
    for (int i =0; i <filas; i++){
        for (int j = 0; j < columnas; j++) {
            // Si el valor es 1 (Mi PARED), se dibuja el numero con #
            if (matriz[i][j]== PARED){
                cout << "##";
            }
            else {
                cout << "  ";
            }
        }
        cout << endl; // Salto de linea al terminar cada fila
    }
}
void generarLaberinto(int** matriz, int filas, int columnas){  // Aqui es donde se genera mi laberinto
    // Preparo la mochila (pila) y el punto de donde partira
    stack<Punto> pila;
    Punto inicio = {1,1};
    matriz[inicio.y][inicio.x]= CAMINO;
    pila.push(inicio);
    // "Semilla" para que cada laberinto sea distinto
    srand(time(NULL));
    while (!pila.empty()) {
        Punto actual = pila.top();
        vector<Punto> vecinos;
        // Aqui se busca vecinos validos a 2 pasos de distancia
        for (int i = 0; i < 4; i++) {
            int ny = actual.y + dy[i];
            int nx = actual.x + dx[i];
            // Se verifica que este dentro de los limites y sea pared
            if (ny > 0 && ny < filas - 1 && nx > 0 && nx < columnas -1 && matriz[ny][nx] == PARED) {
              vecinos.push_back({ny,nx}); 
            }
        }
     // Se puede seguir cavando?
     if(!vecinos.empty()) {
        Punto elegido = vecinos[rand() % vecinos.size()];
        // Se rompe la pared intermedia entre el actual y el elegido
        matriz[(actual.y + elegido.y) / 2][(actual.x + elegido.x) / 2] = CAMINO;
        // Aqui se marca el destino como camino y se ingresa a la pila
        matriz[elegido.y][elegido.x] = CAMINO;
        pila.push(elegido);
     }  else {
        // Si no hay salida, retrocede
        pila.pop();
     }
    }
    // Aseguro que la entrada y la salida estan marcadas 
    matriz[1][1] = CAMINO;
    matriz[filas - 2][columnas - 2] = CAMINO;
}
void resolverLaberinto(int** matriz, int filas, int columnas) {   // Aqui es donde le busco la solucion a mi laberinto con BFS
    // Herramienta de navegación 
    queue<Punto> cola;
    // Matriz de 'padres' para recordar de dónde vinimos y reconstruir el camino
    vector<vector<Punto>> padre(filas, vector<Punto>(columnas, {-1, -1}));
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));

    Punto inicio = {1, 1};
    Punto fin = {filas - 2, columnas - 2};

    cola.push(inicio);
    visitado[inicio.y][inicio.x] = true;

    // Direcciones de movimiento simple (1 paso a la vez)
    int dy_r[] = {0, 0, 1, -1};
    int dx_r[] = {1, -1, 0, 0};

    bool encontrado = false;

    // Exploración estilo "Onda de Agua"
    while (!cola.empty()) {
        Punto actual = cola.front();
        cola.pop();

        if (actual.y == fin.y && actual.x == fin.x) {
            encontrado = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int ny = actual.y + dy_r[i];
            int nx = actual.x + dx_r[i];

            // Si es camino (0) y no lo hemos visitado
            if (ny >= 0 && ny < filas && nx >= 0 && nx < columnas && 
                matriz[ny][nx] == CAMINO && !visitado[ny][nx]) {
                
                visitado[ny][nx] = true;
                padre[ny][nx] = actual; // Guardo quién es el "padre" de esta celda
                cola.push({ny, nx});
            }
        }
    }

    // Reconstrucción del camino (El rastro de Ariadna)
    if (encontrado) {
        Punto paso = fin;
        while (!(paso.y == inicio.y && paso.x == inicio.x)) {
            matriz[paso.y][paso.x] = RUTA_SOLUCION; // Marcamos con 2
            paso = padre[paso.y][paso.x]; // Retrocedemos al padre
        }
        matriz[inicio.y][inicio.x] = RUTA_SOLUCION; // Marcamos el inicio
    }
}
