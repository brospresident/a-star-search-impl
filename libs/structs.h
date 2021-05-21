typedef struct pair {
    int a;
    int b;
} Pair;

typedef struct ppair {
    double a;
    Pair b;
} pPair;

typedef struct cell {
    int parentI; 
    int parentJ;
    double f;
    double g;
    double h;
} Cell;

Pair makePair(int a, int b) {
    Pair newPair;

    newPair.a = a;
    newPair.b = b;

    return newPair;
}