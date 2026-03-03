# Codepro_Laberinto_Maestro
Implementación de C++
Este proyecto es un generador y resolutor de laberintos perfectos desarrollado en C++. Utiliza algoritmos de búsqueda para crear estructuras complejas y encontrar el camino más corto de forma eficiente.

🚀 Características Principales
Generación Aleatoria: Crea laberintos únicos en cada ejecución utilizando DFS (Depth First Search) con una pila.

Resolución Inteligente: Encuentra la ruta óptima desde la entrada (EE) hasta la salida (SS) mediante BFS (Breadth-First Search).

Memoria Dinámica: Gestión manual de memoria mediante punteros dobles (int**) para un control absoluto de los recur
🧠 Algoritmos Utilizados
1. Generación: DFS (Backtracking)
Se utiliza una Pila (std::stack) para profundizar en la matriz hasta encontrar un callejón sin salida, momento en el que el algoritmo retrocede para explorar nuevas rutas. Esto garantiza que cada celda sea alcanzable y no existan ciclos.

2. Resolución: BFS (Breadth-First Search)
Para la solución, se emplea una Cola (std::queue). Este algoritmo se expande como una onda desde el inicio, asegurando que el primer rastro que toque la salida sea siempre el camino más corto.
📊 Especificaciones Técnicas
Lenguaje: C++

Entrada/Salida: iostream

Estructuras: vector, stack, queue

Medición: chrono para el tiempo de respuesta.