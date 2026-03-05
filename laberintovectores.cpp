#include <iostream>  // Esto permite que ingresen y salgan los datos
#include <vector>    // Matriz y coordenadas dinámicas (Gestión automática de memoria)
#include <stack>     // Estructura LIFO para Generación (Backtracking) "Ultimo en entrar, primero en salir"
#include <queue>     // Estructura FIFO para Resolución (Búsqueda en Amplitud) "Primero en entrar, primero en salir"
#include <ctime>     // Para la semilla del azar
#include <cstdlib>   // Para rand() y srand() "Aleatorio"
#include <chrono>    // Para medir el tiempo de ejecución (Métrica de eficiencia)
#include <utility>   // Para usar 'pair', que guarda dos datos juntos (Y, X)

using namespace std;

// Constantes de estado, facilita el mantenimiento del código y evita errores de lógica.
const int PARED = 1;
const int CAMINO = 0;
const int RUTA_SOLUCION = 2;

// Direcciones de movimiento: Representan Norte, Sur, Este y Oeste.
int dy[] = {0, 0, 1, -1};
int dx[] = {1, -1, 0, 0};

// Función de visualización: Mantiene una cuadrícula perfecta de 2 caracteres por celda.
void imprimirLaberinto(const vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == 1 && j == 1) cout << "EE"; // Entrada
            else if (i == filas - 2 && j == columnas - 2) cout << "SS"; // Salida
            else if (matriz[i][j] == RUTA_SOLUCION) cout << ".."; // Rastro de resolución
            else if (matriz[i][j] == PARED) cout << "##"; // Muro sólido
            else cout << "  "; // Pasillo vacío
        }
        cout << endl;
    }
}

// GENERACIÓN: Algoritmo DFS (Depth First Search)
// -> CONCEPTO: Explora lo más profundo posible antes de retroceder.
void generarLaberinto(vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    stack<pair<int, int>> pila; // LIFO: Para recordar el camino y hacer Backtracking.
    
    matriz[1][1] = CAMINO; // Punto de partida
    pila.push({1, 1});
    srand(time(NULL)); // Semilla basada en el tiempo para laberintos únicos.

    while (!pila.empty()) {
        pair<int, int> actual = pila.top();
        vector<pair<int, int>> vecinos;

        for (int i = 0; i < 4; i++) {
            // -> POR QUÉ * 2: Saltamos dos celdas para dejar una pared intermedia.
            int ny = actual.first + (dy[i] * 2);   // Aqui se multiplica el movimiento por 2 esto obliga a saltar una celda de por medio para que siempre quede una pared divisoria.
            int nx = actual.second + (dx[i] * 2);

            // Verificamos que el salto esté dentro de los límites y sea pared virgen.
            if (ny > 0 && ny < filas - 1 && nx > 0 && nx < columnas - 1 && matriz[ny][nx] == PARED) {
                vecinos.push_back({ny, nx});
            }
        }

        if (!vecinos.empty()) {
            pair<int, int> elegido = vecinos[rand() % vecinos.size()];
            // Rompemos la pared intermedia entre el actual y el elegido.
            matriz[(actual.first + elegido.first) / 2][(actual.second + elegido.second) / 2] = CAMINO;
            matriz[elegido.first][elegido.second] = CAMINO; // Marcamos el nuevo camino.
            pila.push(elegido);
        } else {
            pila.pop(); // BACKTRACKING: No hay salida, retrocedemos al nudo anterior.
        }
    }
}

// RESOLUCIÓN: Algoritmo BFS (Breadth First Search)
// -> CONCEPTO: Explora por capas (como una inundación) para hallar la ruta más corta.
void resolverLaberinto(vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    queue<pair<int, int>> cola; // FIFO: Procesa los puntos en orden de descubrimiento.
    
    // Matriz de padres: Almacena de qué celda venimos para reconstruir el camino al final.
    vector<vector<pair<int, int>>> padre(filas, vector<pair<int, int>>(columnas, {-1, -1}));
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));

    cola.push({1, 1});
    visitado[1][1] = true;

    bool encontrado = false;
    while (!cola.empty()) {
        pair<int, int> actual = cola.front(); // Obtenemos el primero en la fila.
        cola.pop();

        // Si llegamos a la posición de la salida (SS).
        if (actual.first == filas - 2 && actual.second == columnas - 2) {
            encontrado = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int ny = actual.first + dy[i]; // Aquí el movimiento es de 1 en 1.
            int nx = actual.second + dx[i];

            if (ny >= 0 && ny < filas && nx >= 0 && nx < columnas && matriz[ny][nx] == CAMINO && !visitado[ny][nx]) {
                visitado[ny][nx] = true;
                padre[ny][nx] = actual; // Registramos la procedencia.
                cola.push({ny, nx});
            }
        }
    }

    if (encontrado) {
        // RECONSTRUCCIÓN: Seguimos los pasos desde el final hacia el inicio.
        pair<int, int> paso = padre[filas - 2][columnas - 2];
        while (paso.first != 1 || paso.second != 1) {
            matriz[paso.first][paso.second] = RUTA_SOLUCION;
            paso = padre[paso.first][paso.second];
        }
    }
}

int main() {
    int f, c;
    cout << "=== Laberinto Maestro ===" << endl;
    cout << "Filas: "; cin >> f;
    cout << "Columnas: "; cin >> c;

    // Ajuste de paridad: Garantiza que el diseño Pared-Camino-Pared funcione.
    if (f % 2 == 0) f++;
    if (c % 2 == 0) c++;

    // MATRIZ DINÁMICA STL: Se inicializa con 'filas' vectores, cada uno de tamaño 'columnas'.
    // Aqui la memoria se libera automáticamente al terminar el programa.
    vector<vector<int>> lab(f, vector<int>(c, PARED));

    generarLaberinto(lab);
    
    auto start = chrono::high_resolution_clock::now(); // Aqui inicia el cronómetro.
    resolverLaberinto(lab);
    auto end = chrono::high_resolution_clock::now(); // Aqui finaliza el cronómetro.

    // Aqui utilizo duration_cast para convertir la diferencia a microsegundos (us). 
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    imprimirLaberinto(lab);
    
    // Mostramos el resultado como un número entero
    cout << "\nResolucion completada en: " << duration.count() << " microsegundos (us)." << endl;  // "us" Un microsegundo es la millonesima parte de un segundo

    return 0; // El programa termina y los vectores limpian la memoria automaticamente
}