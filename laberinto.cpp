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
// Aqui creo la matriz del labertinto
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