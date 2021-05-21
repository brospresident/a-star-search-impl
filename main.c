#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#include "./libs/misc.h"
#include "./libs/structs.h"
#include "./libs/stacks.h"

bool isValid(int row, int col) {
    return row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLUMNS;
}

bool canPass(int grid[GRID_ROWS][GRID_COLUMNS], int row, int col) {
    return grid[row][col] != OBSTACLE ? true : false;
}

bool foundDestination(int grid[GRID_ROWS][GRID_COLUMNS], int row, int col) {
    return grid[row][col] == END ? true : false;
}

double calcEuclidian(int row, int col, Pair destination) {
    return sqrt(pow(row - destination.a, 2) + pow(col - destination.b, 2));
}

void printPath(Cell cells[][GRID_COLUMNS], Pair destination) {
    printf("Calea ");
    int r = destination.a;
    int c = destination.b;

    Stack* path = createStack(GRID_COLUMNS * GRID_ROWS);

    while (!cells[r][c].parentI == r && !cells[r][c].parentJ == c) {
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
        temp = top(path);
        pop(path);
        printf("-> (%d, %d) ", temp.a, temp.b);
    }
}

bool checkUnique(pPair* vec, int obj, pPair data) {
    if (obj == 0) return true;

    for (int i = 0; i < obj; ++i) {
        if (vec[i].a == data.a 
        && vec[i].b.a == data.b.a 
        && vec[i].b.b == data.b.b) {
            return false;
        }
    }

    return true;
}

void aStar(int grid[GRID_ROWS][GRID_COLUMNS], Pair src, Pair dest) {
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

    bool closedList[GRID_ROWS][GRID_COLUMNS];
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            closedList[i][j] = false;
        }
    }

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

    int i = src.a, j = src.b;
    cells[i][j].f = 0.0;
    cells[i][j].g = 0.0;
    cells[i][j].h = 0.0;
    cells[i][j].parentI = i;
    cells[i][j].parentJ = j;

    pPair* openList = (pPair*)malloc(sizeof(pPair) * (GRID_COLUMNS * GRID_ROWS));
    int ocupate = 0;

    openList[ocupate].a = 0.0;
    openList[ocupate].b.a = i;
    openList[ocupate].b.b = j;
    ocupate++;

    bool foundDest = false;

    while (ocupate != 0) {
        pPair p = openList[ocupate];
        ocupate--;

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

        double gNew, hNew, fNew;

        // NORD
        if (isValid(i - 1, j)) {
            if (foundDestination(grid, i - 1, j)) {
                cells[i - 1][j].parentI = i;
                cells[i - 1][j].parentJ = j;
                printf("Am gasit destinatia.\n");
                printPath(cells, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i - 1][j] == false && canPass(grid, i - 1, j)) {
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i - 1, j, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j].f == FLT_MAX || cells[i - 1][j].f > fNew) {
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i - 1;
                    openList[ocupate].b.b = j;
                    ocupate++;

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
            printf("TEST");
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
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i = 1;
                    openList[ocupate].b.b = j;
                    ocupate++;

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
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i;
                    openList[ocupate].b.b = j + 1;
                    ocupate++;

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
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i;
                    openList[ocupate].b.b = j - 1;
                    ocupate++;

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
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j + 1].f == FLT_MAX || cells[i - 1][j + 1].f > fNew) {
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i - 1;
                    openList[ocupate].b.b = j + 1;
                    ocupate++;

                    cells[i - 1][j + 1].f = fNew;
                    cells[i - 1][j + 1].g = gNew;
                    cells[i - 1][j + 1 ].h = hNew;
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
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cells[i - 1][j - 1].f == FLT_MAX || cells[i - 1][j - 1].f > fNew) {
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i - 1;
                    openList[ocupate].b.b = j - 1;
                    ocupate++;

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
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cells[i + 1][j - 1].f == FLT_MAX || cells[i + 1][j - 1].f > fNew) {
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i + 1;
                    openList[ocupate].b.b = j - 1;
                    ocupate++;

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
                gNew = cells[i][j].g + 1.0;
                hNew = calcEuclidian(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cells[i + 1][j - 1].f == FLT_MAX || cells[i + 1][j + 1].f > fNew) {
                    openList[ocupate].a = fNew;
                    openList[ocupate].b.a = i + 1;
                    openList[ocupate].b.b = j + 1;
                    ocupate++;

                    cells[i + 1][j + 1].f = fNew;
                    cells[i + 1][j + 1].g = gNew;
                    cells[i + 1][j + 1].h = hNew;
                    cells[i + 1][j + 1].parentI = i;
                    cells[i + 1][j + 1].parentJ = j;
                }
            }
        }
    }

    if (!foundDest) {
        printf("Nu am putut sa gasesc destinatia...");
    }
}

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