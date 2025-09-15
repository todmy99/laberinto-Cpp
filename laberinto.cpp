#include <iostream> //para cin, cout
#include <cstdlib> // para rand()
#include <ctime> //para srand(time(0))
#include <vector> //para usar vectores
#include <utility> //por std::pair
using namespace std; //evitar escribir std:: cada vez //no es considerado buena practica para proyectos grandes

//mezclar direcciones, para que el laberinto sea aleatorio
static inline void mezclar4(int dirs[4]) {
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);
        int t = dirs[i]; dirs[i] = dirs[j]; dirs[j] = t;
    }
}

//convierte la celda cx,cy en coordenadas en la matriz x,y (en posiciones impares)
std::pair<int,int> cell2grid(int cx, int cy) {
    return { 1 + 2*cx, 1 + 2*cy };
}

//crear pasillos con dfs
void carveCells(std::vector<std::vector<char>>& m,
                int cx, int cy, int cellRows, int cellCols) {
    auto [mx, my] = cell2grid(cx, cy);
    m[mx][my] = ' ';  //la celda actual pasa a ser pasillo

    int dcx[4] = {-1, 1, 0, 0};
    int dcy[4] = { 0, 0,-1, 1};

    int dirs[4] = {0,1,2,3};
    mezclar4(dirs);  //orden aleatorio

    for (int t = 0; t < 4; ++t) {
        int dir = dirs[t];
        int ncx = cx + dcx[dir];
        int ncy = cy + dcy[dir];

        //limites para las celdas
        if (ncx < 0 || ncy < 0 || ncx >= cellRows || ncy >= cellCols) continue;

        auto [nmx, nmy] = cell2grid(ncx, ncy);
        if (m[nmx][nmy] == '#') {   //celda vecina no visitada
            // abrir la pared intermedia
            int wx = (mx + nmx) / 2;
            int wy = (my + nmy) / 2;
            m[wx][wy] = ' ';
            m[nmx][nmy] = ' ';

            carveCells(m, ncx, ncy, cellRows, cellCols);
        }
    }
}

//genera el laberinto
void generarLaberinto(std::vector<std::vector<char>>& m) {
    int f = (int)m.size();
    int c = (int)m[0].size();

    //calcula cuantas celdas hay
    int cellFilas = (f - 1) / 2;
    int cellColumnas = (c - 1) / 2;

    //arrancamos en la celda (0,0) -> matriz (1,1)
    carveCells(m, 0, 0, cellFilas, cellColumnas);

    //conectar esquinas para S y E
    m[0][0] = ' ';            if (f > 1) m[1][0] = ' '; if (c > 1) m[0][1] = ' ';
    m[f-1][c-1] = ' ';        if (f > 1) m[f-2][c-1] = ' '; if (c > 1) m[f-1][c-2] = ' ';
}

bool resolverLaberinto (vector<vector<char>>& matriz, int x, int y) { //funcion, solo necesita saber el numero de columnas
    int filas = matriz.size();
    int columnas = matriz [0].size();

    if (x < 0 || y < 0 || x >= filas || y >= columnas) //limites de la matriz
        return false;

    if (matriz[x][y] == 'E') //llegamos a la salida, termina la recursion
        return true;

    if (matriz[x][y] != ' ' && matriz[x][y] != 'S') //solo podemos pasar por celdas vacias o la entrada
        return false;

    if (matriz[x][y] == ' ') 
        matriz[x][y] = '*'; //marcamos el recorrido con *

    //intenta moverse en las 4 direcciones
    if (resolverLaberinto(matriz, x+1, y)) return true; //abajo
    if (resolverLaberinto(matriz, x-1, y)) return true; //arriba
    if (resolverLaberinto(matriz, x, y+1)) return true; //derecha
    if (resolverLaberinto(matriz, x, y-1)) return true; //izquierda

    //si no se puede ninguno de esos, retrocede (backtracking)
    if (matriz[x][y] == '*') matriz[x][y] = ' ';
    return false;
}

//mi bucle principal
int main() {
    srand(time(0)); //cada ejecucion genera un camino distinto
    int filas, columnas; // declarar variables

    do { //validar
        cout << "introduce el numero de filas (de 3 para arriba): ";
        cin >> filas;
    } while (filas < 3);

    do {
        cout << "introduce el numero de columnas (de 3 para arriba): ";
        cin >> columnas;
    } while (columnas < 3);


    //declara la matriz 2d
    vector<vector<char>> matriz(filas, vector<char>(columnas,'#')); //char declara una variable de tipo caracter, crea un vector de elementos para filas y columnas
    generarLaberinto(matriz);

    matriz[0][0] = 'S'; //marco start y exit
    matriz[filas -1][columnas -1] = 'E';

    resolverLaberinto(matriz, 0, 0);

    cout << "\nlaberinto resuelto:\n"; //aparentemente se usa "" para cadenas de texto
    for (int i = 0; i < filas; i++) { // recorre filas
        for (int j = 0; j < columnas; j++) { //recorre columnas
            cout << matriz[i][j] << ' '; // ' ' espacio
        }
        cout << endl; //cout = imprime en pantalla, endl = salto de linea
    }

    return 0; //investigar
}