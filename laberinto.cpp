#include <iostream> // Esto permite que ingresen y salgan los datos
#include <vector> // Esto permite utilizar los vectores dinamicos o "matrices"
#include <stack> // Se utiliza en el algoritmo de generacion (backtracking/dfs)
#include <queue> // Se utiliza en el algoritmo de resolucion del laberinto (bfs)
#include <ctime> // Permite acceder al reloj del sistema 
#include <cstdlib> // Contiene la funciones de aletoriedad
#include <chrono> // Para medir el tiempo de ejecución (Requisito Penguin)

using namespace std;

// Estructura para manejar las coordenadas (Y, X)
struct Punto {
    int y, x;
};

// Variables constantes 
const int PARED = 1;
const int CAMINO = 0;
const int RUTA_SOLUCION = 2;

// Direcciones de movimiento para GENERAR (salta de 2 en 2)
int dy_gen[] = {0, 0, 2, -2};
int dx_gen[] = {2, -2, 0, 0};

// Aqui creo la matriz del labertinto "Aqui tambien le solicito la memoria a utilizar"
int** crearMatriz(int filas, int columnas) {
    int** matriz = new int*[filas];
    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = PARED;
        }
    }
    return matriz;
}

void destruirMatriz(int** matriz, int filas) {
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void imprimirLaberinto(int** matriz, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            // REGLA DE ORO: Todos los cout deben imprimir exactamente 2 CARACTERES
            if (i == 1 && j == 1) {
                cout << "EE"; // Entrada
            } 
            else if (i == filas - 2 && j == columnas - 2) {
                cout << "SS"; // Salida
            }
            else if (matriz[i][j] == RUTA_SOLUCION) {
                cout << ".."; // Camino de solución
            }
            else if (matriz[i][j] == PARED) {
                cout << "##"; // Paredes
            }
            else {
                cout << "  "; // Camino vacío (¡2 espacios, no 3!)
            }
        }
        cout << endl;
    }
}

void generarLaberinto(int** matriz, int filas, int columnas) {
    stack<Punto> pila;
    Punto inicio = {1, 1};
    matriz[inicio.y][inicio.x] = CAMINO;
    pila.push(inicio);
    srand(time(NULL));

    while (!pila.empty()) {
        Punto actual = pila.top();
        vector<Punto> vecinos;

        for (int i = 0; i < 4; i++) {
            int ny = actual.y + dy_gen[i];
            int nx = actual.x + dx_gen[i];

            if (ny > 0 && ny < filas - 1 && nx > 0 && nx < columnas - 1 && matriz[ny][nx] == PARED) {
                vecinos.push_back({ny, nx});
            }
        }

        if (!vecinos.empty()) {
            Punto elegido = vecinos[rand() % vecinos.size()];
            matriz[(actual.y + elegido.y) / 2][(actual.x + elegido.x) / 2] = CAMINO;
            matriz[elegido.y][elegido.x] = CAMINO;
            pila.push(elegido);
        } else {
            pila.pop();
        }
    }
}

void resolverLaberinto(int** matriz, int filas, int columnas) {
    queue<Punto> cola;
    vector<vector<Punto>> padre(filas, vector<Punto>(columnas, {-1, -1}));
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));

    Punto inicio = {1, 1};
    Punto fin = {filas - 2, columnas - 2};

    cola.push(inicio);
    visitado[inicio.y][inicio.x] = true;

    // Movimiento de 1 en 1 para RESOLVER (no atraviesa paredes)
    int dy_res[] = {0, 0, 1, -1};
    int dx_res[] = {1, -1, 0, 0};

    bool encontrado = false;

    while (!cola.empty()) {
        Punto actual = cola.front();
        cola.pop();

        if (actual.y == fin.y && actual.x == fin.x) {
            encontrado = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int ny = actual.y + dy_res[i];
            int nx = actual.x + dx_res[i];

            if (ny >= 0 && ny < filas && nx >= 0 && nx < columnas && 
                matriz[ny][nx] == CAMINO && !visitado[ny][nx]) {
                
                visitado[ny][nx] = true;
                padre[ny][nx] = actual;
                cola.push({ny, nx});
            }
        }
    }

    if (encontrado) {
        Punto paso = padre[fin.y][fin.x]; // Empezamos desde el padre de la salida
        while (!(paso.y == inicio.y && paso.x == inicio.x)) {
            matriz[paso.y][paso.x] = RUTA_SOLUCION;
            paso = padre[paso.y][paso.x];
        }
    }
}

// MAIN con soporte para parámetros externos (argc y argv)
int main(int argc, char* argv[]) {
    int f = 15, c = 15; // Valores por defecto

    // Si pasas argumentos por consola por ejemplo .\laberinto.exe 25 35)
    if (argc >= 3) {
        f = atoi(argv[1]);
        c = atoi(argv[2]);
    }

    // El laberinto DEBE tener dimensiones impares
    if (f % 2 == 0) f++;
    if (c % 2 == 0) c++;

    int** lab = crearMatriz(f, c);
    
    generarLaberinto(lab, f, c);

    // Medición de tiempo (Requisito de optimización)
    auto start = chrono::high_resolution_clock::now();
    resolverLaberinto(lab, f, c); 
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    cout << "\n--- LABERINTO MAESTRO (" << f << "x" << c << ") ---" << endl;
    imprimirLaberinto(lab, f, c);
    
    cout << "\nResolucion completada en: " << elapsed.count() << " segundos." << endl;

    destruirMatriz(lab, f);
    return 0;
}