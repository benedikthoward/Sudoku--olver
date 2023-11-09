#include <stdio.h>
#include "SudokuStuff.h"

typedef struct Generational_state{
    unsigned char generational_field[9][9];
    unsigned char possibilities[9][9][9];
}Generational_state;

typedef struct Generation{
    Generational_state *offspring;
}Generation;

int main() {
    unsigned char sud[9][9] = {{0, 3, 0, 9, 2, 0, 4, 0, 1},
                     {0, 1, 0, 3, 0, 4, 0, 0, 0},
                     {4, 0, 7, 6, 0, 0, 0, 9, 3},
                     {0, 0, 0, 0, 0, 0, 0, 6, 5},
                     {1, 0, 0, 7, 9, 0, 3, 0, 4},
                     {0, 7, 0, 0, 4, 0, 2, 1, 9},
                     {7, 5, 0, 4, 6, 0, 0, 3, 0},
                     {3, 0, 6, 0, 5, 1, 0, 4, 0},
                     {0, 4, 0, 0, 7, 3, 0, 0, 0}};

    unsigned char possibilities [9][9][9];
    //fill_conclusive returns if it made a change based on a field that is actually known 
    while(fill_conclusive(possibilities,sud)) {
        fill_possibilities(possibilities,sud);
    }

    int decision[] =  decision_Array(possibilities,sud);
    Generation gen1;
    gen1.offspring = (Generational_state *)malloc(decision[2] * sizeof(Generational_state));
    for(int i = 0;i<decision_Array(possibilities,sud)[2]; i++){
        gen1.offspring[i].generational_field = decision_copy_array(decision[1],decision[0],decision[i+3],sud);
    }
    return 0;
}