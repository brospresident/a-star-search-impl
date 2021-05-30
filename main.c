#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

// contine niste define-uri pentru variabile
#include "./libs/misc.h"

// contine structurile folosite
#include "./libs/structs.h"

// contine o implementare de stack folosind structura Pair din fisierul structs.h
#include "./libs/stacks.h"


/**
 * Functia verifica daca anumite coordonate sunt parte din harta primita ca input
 * @param row linia
 * @param col coloana
 * @retval intoarce daca sunt valabile coordonatele
*/
bool isValid(int row, int col) {
    return row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLUMNS;
}


/**
 * Functia verifica daca pe o casuta exista sau nu obstacol, acesta e marcat pe harta cu OBSTACLE(se gaseste cine e OBSTACLE in libs/misc.h)
 * @param grid matricea in care stochez harta
 * @param row linia
 * @param col coloana
 * @retval intoarce daca se poate trece prin acel punct de pe harta
*/
bool canPass(int grid[][GRID_COLUMNS], int row, int col) {
    return grid[row][col] != OBSTACLE ? true : false;
}


/**
 * Functie care verifica daca am ajuns la destinatie, aceasta este marcata cu END in matrice(se gaseste cine e END in libs/misc.h)
 * @param grid harta primita ca input
 * @param row linia
 * @param col coloana
 * @retval daca am gasit sau nu destinatia
*/
bool foundDestination(int grid[][GRID_COLUMNS], int row, int col) {
    return grid[row][col] == END ? true : false;
}


/**
 * Functie care calculeaza distanta euclidiana
 * @param row linia de la care calculam
 * @param col coloana de la care calculam
 * @param destination destinatia
 * @retval distanta euclidiana
*/
double calcEuclidian(int row, int col, Pair destination) {
    return sqrt(pow(row - destination.a, 2) + pow(col - destination.b, 2));
}


/**
 * Functie care afiseaza calea gasita de algoritmul A*
 * @param cells matrice in care am stocat detaliile fiecarei casute
 * @param destination destinatia la care vrem sa ajungem
 * @retval nu intoarce 
*/
void printPath(Cell cells[][GRID_COLUMNS], Pair destination) {
    printf("Calea ");
    int r = destination.a;
    int c = destination.b;

    Stack* path = createStack(GRID_COLUMNS * GRID_ROWS);

    while (!(cells[r][c].parentI == r && cells[r][c].parentJ == c)) {
        Pair temp;
        temp.a = r;
        temp.b = c;

        push(path, temp);

        int R = cells[r][c].parentI;
        int C = cells[r][c].parentJ;

        r = R;
        c = C;
    }

    Pair temp;
    temp.a = r;
    temp.b = c;

    push(path, temp);
    while (!isEmpty(path)) {
        temp = path->data[path->occupied];
        pop(path);
        printf("-> (%d, %d) ", temp.a, temp.b);
    }
}

/**
 * Functie care verifica daca un element exista deja in openList
 * @param vec openList-ul
 * @param obj cate elemente contine lista
 * @retval daca exista sau nu acel element
*/
bool checkUnique(pPair* vec, int obj, int f, int a, int b) {
    if (obj == 0) return true;

    for (int i = 0; i < obj; ++i) {
        if (vec[i].a == f
        && vec[i].b.a == a 
        && vec[i].b.b == b) {
            return false;
        }
    }

    return true;
}

/**
 * Functie care face cautarea A*
 * @param grid harta primita ca input
 * @param src punctul din care plecam
 * @param dest destinatia la care vrem sa ajungem
*/
void aStar(int grid[][GRID_COLUMNS], Pair src, Pair dest) {
    // diverse verificari pentru punct
    if (!isValid(src.a, src.b)) {
        printf("Sursa invalida\n");
        return;
    }

    if (!isValid(dest.a, dest.b)) {
        printf("Destinatie invalida\n");
        return;
    }

    if (!canPass(grid, src.a, src.b)) {
        printf("Sursa este blocata\n");
        return;
    }

    if (!canPass(grid, dest.a, dest.b)) {
        printf("Destinatia este blocata\n");
        return;
    }

    // declaram un hashtable in care vom marca daca am vizitat sau nu un nod
    bool closedList[GRID_ROWS][GRID_COLUMNS];
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            closedList[i][j] = false;
        }
    }

    // declaram o matrice in care stocam toate detaliile unui punct de pe harta
    Cell cells[GRID_ROWS][GRID_COLUMNS];

    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            cells[i][j].f = FLT_MAX;
            cells[i][j].g = FLT_MAX;
            cells[i][j].h = FLT_MAX;
            cells[i][j].parentI = -1;
            cells[i][j].parentJ = -1;
        }
    }

    // initializez parametrii primului nod(sursa)
    int i = src.a, j = src.b;
    cells[i][j].f = 0.0;
    cells[i][j].g = 0.0;
    cells[i][j].h = 0.0;
    cells[i][j].parentI = i;
    cells[i][j].parentJ = j;

    // declar un openList, aceasta e formata dintr-un parametru double si un parametru Pair(se gasesc in libs/misc.h)
    pPair* openList = (pPair*)malloc(sizeof(pPair) * (GRID_COLUMNS * GRID_ROWS));
    int ocupate = 0;

    // pun punctul de inceput in openList
    openList[ocupate].a = 0.0;
    openList[ocupate].b.a = i;
    openList[ocupate].b.b = j;
    ocupate++;

    // variabila in care stocam daca am gaasit destinatia, false la inceput
    bool foundDest = false;

    while (ocupate != 0) {
        // scoatem elementul din openList si il trecem in closedList
        pPair p = openList[--ocupate];

        i = p.b.a;
        j = p.b.b;
        closedList[i][j] = true;

       /**
        * Generez toti succesorii acestei celule pe toate directiile
             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \ 
                /   |  \
             S.W    S   S.E
        cell->popped(i, j)
        N->Nord (i - 1, j)
        S->Sud (i + 1, j)
        E->Est (i, j + 1)
        W->Vest (i, j - 1)
        N.E->Nord-Est (i - 1, j + 1)
        N.W->Nord-Vest (i - 1, j - 1)
        S.E->Sud-Est (i + 1, j + 1)
        S.W->Sud-Vest (i + 1, j - 1)
       */

        double gNew = 0.0, hNew = 0.0, fNew = 0.0;

        // NORD
        // fac verificari ulterioare doar daca punctul e valid
        if (isValid(i - 1, j)) {
            if (foundDestination(grid, i - 1, j)) { // daca punctull de destinatie e acelasi cu succesorul
                cells[i - 1][j].parentI = i;
                cells[i - 1][j].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            } // daca succesorul e deja in closedList atunci nu fac nimic altfel il adaug
            else if (closedList[i - 1][j] == false && canPass(grid, i - 1, j)) {
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i - 1, j, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j].f == FLT_MAX || cells[i - 1][j].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i - 1, j) == true) { // verific daca este unic
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i - 1;
                        openList[ocupate].b.b = j;
                        ocupate++;
                    }
                    
                    cells[i - 1][j].f = fNew;
                    cells[i - 1][j].g = gNew;
                    cells[i - 1][j].h = hNew;
                    cells[i - 1][j].parentI = i;
                    cells[i - 1][j].parentJ = j;
                    
                }
            }
        }

        // SUD
        if (isValid(i + 1, j)) {
            if (foundDestination(grid, i + 1, j)) {
                cells[i + 1][j].parentI = i;
                cells[i + 1][j].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i + 1][j] == false && canPass(grid, i + 1, j)) {
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i + 1, j, dest);
                fNew = gNew + hNew;

                if (cells[i + 1][j].f == FLT_MAX || cells[i + 1][j].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i - 1, j)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i;
                        openList[ocupate].b.b = j;
                        ocupate++;
                    }
                    
                    cells[i + 1][j].f = fNew;
                    cells[i + 1][j].g = gNew;
                    cells[i + 1][j].h = hNew;
                    cells[i + 1][j].parentI = i;
                    cells[i + 1][j].parentJ = j;
                   
                }
            }
        }

        // EST
        if (isValid(i, j + 1)) {
            if (foundDestination(grid, i, j + 1)) {
                cells[i][j + 1].parentI = i;
                cells[i][j + 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j + 1] == false && canPass(grid, i, j + 1)) {
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i, j + 1, dest);
                fNew = gNew + hNew;

                if (cells[i][j + 1].f == FLT_MAX || cells[i][j + 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i, j + 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i;
                        openList[ocupate].b.b = j + 1;
                        ocupate++;
                    }
                    
                    cells[i][j + 1].f = fNew;
                    cells[i][j + 1].g = gNew;
                    cells[i][j + 1].h = hNew;
                    cells[i][j + 1].parentI = i;
                    cells[i][j + 1].parentJ = j;
                    
                }
            }
        }

        // VEST
        if (isValid(i, j - 1)) {
            if (foundDestination(grid, i, j - 1)) {
                cells[i][j - 1].parentI = i;
                cells[i][j - 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j - 1] == false && canPass(grid, i, j - 1)) {
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i, j - 1, dest);
                fNew = gNew + hNew;

                if (cells[i][j - 1].f == FLT_MAX || cells[i][j - 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i, j - 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i;
                        openList[ocupate].b.b = j - 1;
                        ocupate++;
                    }

                    cells[i][j - 1].f = fNew;
                    cells[i][j - 1].g = gNew;
                    cells[i][j - 1].h = hNew;
                    cells[i][j - 1].parentI = i;
                    cells[i][j - 1].parentJ = j;

                    
                }
            }
        }

        // NORD-EST
        if (isValid(i - 1, j + 1)) {
            if (foundDestination(grid, i - 1, j + 1)) {
                cells[i - 1][j + 1].parentI = i;
                cells[i - 1][j + 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i - 1][j + 1] == false && canPass(grid, i - 1, j + 1)) {
                gNew = cells[i][j].g + 1.414;
                hNew = calcEuclidian(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j + 1].f == FLT_MAX || cells[i - 1][j + 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i - 1, j + 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i - 1;
                        openList[ocupate].b.b = j + 1;
                        ocupate++;
                    }
                    cells[i - 1][j + 1].f = fNew;
                    cells[i - 1][j + 1].g = gNew;
                    cells[i - 1][j + 1].h = hNew;
                    cells[i - 1][j + 1].parentI = i;
                    cells[i - 1][j + 1].parentJ = j;
                    
                    
                }
            }
        }

        // NORD-VEST
        if (isValid(i - 1, j - 1)) {
            if (foundDestination(grid, i - 1, j - 1)) {
                cells[i - 1][j - 1].parentI = i;
                cells[i - 1][j - 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i - 1][j - 1] == false && canPass(grid, i - 1, j - 1)) {
                gNew = cells[i][j].g + 1.414;
                hNew = calcEuclidian(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j - 1].f == FLT_MAX || cells[i - 1][j - 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i - 1, j - 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i - 1;
                        openList[ocupate].b.b = j - 1;
                        ocupate++;
                    }
                    cells[i - 1][j - 1].f = fNew;
                    cells[i - 1][j - 1].g = gNew;
                    cells[i - 1][j - 1].h = hNew;
                    cells[i - 1][j - 1].parentI = i;
                    cells[i - 1][j - 1].parentJ = j;

                    
                }
            }

        }

        // SUD-VEST
        if (isValid(i + 1, j - 1)) {
            if (foundDestination(grid, i + 1, j - 1)) {
                cells[i + 1][j - 1].parentI = i;
                cells[i + 1][j - 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i + 1][j - 1] == false && canPass(grid, i + 1, j - 1)) {
                gNew = cells[i][j].g + 1.414;
                hNew = calcEuclidian(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cells[i + 1][j - 1].f == FLT_MAX || cells[i + 1][j - 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i + 1, j - 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i + 1;
                        openList[ocupate].b.b = j - 1;
                        ocupate++;
                    }

                    cells[i + 1][j - 1].f = fNew;
                    cells[i + 1][j - 1].g = gNew;
                    cells[i + 1][j - 1].h = hNew;
                    cells[i + 1][j - 1].parentI = i;
                    cells[i + 1][j - 1].parentJ = j;

                }
            }
        }

        //SUD-EST
        if (isValid(i + 1, j + 1)) {
            if (foundDestination(grid, i + 1, j + 1)) {
                cells[i + 1][j + 1].parentI = i;
                cells[i + 1][j + 1].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i + 1][j + 1] == false && canPass(grid, i + 1, j + 1)) {
                gNew = cells[i][j].g + 1.414;
                hNew = calcEuclidian(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cells[i + 1][j - 1].f == FLT_MAX || cells[i + 1][j + 1].f > fNew) {
                    if (checkUnique(openList, ocupate, fNew, i + 1, j + 1)) {
                        openList[ocupate].a = fNew;
                        openList[ocupate].b.a = i + 1;
                        openList[ocupate].b.b = j + 1;
                        ocupate++;
                    }

                    cells[i + 1][j + 1].f = fNew;
                    cells[i + 1][j + 1].g = gNew;
                    cells[i + 1][j + 1].h = hNew;
                    cells[i + 1][j + 1].parentI = i;
                    cells[i + 1][j + 1].parentJ = j;

                }
            }
        }
    }

    // daca nu am gasit destinatia afisez
    if (!foundDest) {
        printf("Nu am putut sa gasesc destinatia...");
    }
}

/**
 * Functie care gaseste punctul de start
 * @param grid harta primita ca input
 * @retval punctul de start
*/
Pair findStart(int grid[][GRID_COLUMNS]) {
    Pair res;
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            if (grid[i][j] == START) {
                res.a = i;
                res.b = j;
            }
        }
    }
    return res;
}

/**
 * Functie care gaseste destinatia
 * @param grid harta primita ca input
 * @retval destinatia
*/
Pair findDest(int grid[][GRID_COLUMNS]) {
    Pair res;
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            if (grid[i][j] == END) {
                res.a = i;
                res.b = j;
            }
        }
    }
    return res;
}

int main () {
    int grid[GRID_ROWS][GRID_COLUMNS] = {
        { START, OBSTACLE, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
        { 1, OBSTACLE, 1, OBSTACLE, 1, 1 },
        { 1, OBSTACLE, 1, 1, OBSTACLE, 1 },
        { 1, 1, 1, 1, OBSTACLE, END }
    };

    Pair start = findStart(grid);
    Pair destination = findDest(grid);

    aStar(grid, start, destination);
    return 0;
}