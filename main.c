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
}

int main () {
    int grid[GRID_ROWS][GRID_COLUMNS] = {
        { 1, OBSTACLE, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
        { 1, OBSTACLE, 1, OBSTACLE, 1, 1 },
        { 1, OBSTACLE, 1, 1, OBSTACLE, 1 },
        { 1, 1, 1, 1, OBSTACLE, 1 }
    };
    return 0;
}