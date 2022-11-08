#include <stdio.h>
#include "SudokuStuff.h"



int main() {
    int sud[9][9] = {{0, 3, 0, 9, 2, 0, 4, 0, 1},
                     {0, 1, 0, 3, 0, 4, 0, 0, 0},
                     {4, 0, 7, 6, 0, 0, 0, 9, 3},
                     {0, 0, 0, 0, 0, 0, 0, 6, 5},
                     {1, 0, 0, 7, 9, 0, 3, 0, 4},
                     {0, 7, 0, 0, 4, 0, 2, 1, 9},
                     {7, 5, 0, 4, 6, 0, 0, 3, 0},
                     {3, 0, 6, 0, 5, 1, 0, 4, 0},
                     {0, 4, 0, 0, 7, 3, 0, 0, 0}};
//get the user to input the sudoku problem



    //prints the sudoku problem
    int unknowns = 0;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            printf("%d", sud[r][c]);
            if (sud[r][c] == 0) {
                unknowns++;
            }
            if (c < 8) {
                printf("%s", "|");
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    int solMatrix[28][82];
//fill the matrix with zeros
    for (int r = 0; r < 28; r++) {
        for (int c = 0; c < 82; c++) {
            solMatrix[r][c] = 0;
        }

    }

// fill the "known column" with the maximum sum of a row, column, box
    solMatrix[0][unknowns] = -1;
    for (int i = 1; i < 28; i++) {
        solMatrix[i][unknowns] = 45;
    }

//fills the equation matrix with ones for the unknowns and o for knowns. knowns also get subtractd from the total for the equation
    int unknownsPlaced = 0;
    int knownsPlaced = 0;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {

            if (sud[r][c] == 0) {
                solMatrix[0][unknownsPlaced] = (r * 9) + c;
                //places the value into the row equation
                solMatrix[r + 1][unknownsPlaced] = 1;
                //places the value into the column equation
                solMatrix[c + 10][unknownsPlaced] = 1;
                //places the value into the bo equation
                solMatrix[19 + box(r, c)][unknownsPlaced] = 1;
                unknownsPlaced++;

            } else {
                solMatrix[0][unknowns + knownsPlaced + 1] = (r * 9) + c;
                //places the value into the row equation
                solMatrix[r + 1][unknowns + knownsPlaced + 1] = sud[r][c];
                solMatrix[r + 1][unknowns] -= sud[r][c];
                //places the value into the column equation
                solMatrix[c + 10][unknowns + knownsPlaced + 1] = sud[r][c];
                solMatrix[c + 10][unknowns] -= sud[r][c];
                //places the value into the bo equation
                solMatrix[19 + box(r, c)][unknowns + knownsPlaced + 1] = sud[r][c];
                solMatrix[19 + box(r, c)][unknowns] -= sud[r][c];
                knownsPlaced++;
            }
        }
    }
    //prints the sudoku problem Matrix
    for (int r = 0; r < 28; r++) {
        for (int c = 0; c < 82; c++) {
            printf("%d", solMatrix[r][c]);
            if (c < 81) {
                printf("%s", "|");
            }
        }
        printf("\n");
    }

    int indexOfGuesses[80];
    int numberOfGuesses = 0;
    int acceptableValue;
    for (int l = 0; l < 80; l++) {
        indexOfGuesses[l] = -1;
    }

//finds a combination on columns that keeps all 27 equations in tact and gets down to 27 unknowns. that matrix will be re-arranged
    for (int i = 0; i < unknowns; i++) {
        acceptableValue = 1;
        for (int j = 1; j < 28; j++) {
            int rowTotal = 0;
            for (int k = 0; k < unknowns; k++) {
                if (doesNotContain(solMatrix[0][k], indexOfGuesses) == 1 && k != i) {
                    rowTotal += solMatrix[j][k];

                }
            }
            if (rowTotal == 0) {
                acceptableValue = 0;
                break;

            }
        }
        if (acceptableValue == 1) {
            indexOfGuesses[numberOfGuesses] = solMatrix[0][i];
            numberOfGuesses++;
        }
        if (numberOfGuesses == unknowns - 27) {
            break;
        }
    }

    printf("\n\n");//prints the columns indices that satisfy the directions given above.
    for (int l = 0; l < 80; l++) {
        printf("%d,", indexOfGuesses[l]);
    }


    int solM2[28][82];
    int solM2Filled = 0;

    printf("\n\n");
    
    for(int i=0;i<unknowns;i++){
        if(doesNotContain(solMatrix[0][i],indexOfGuesses)==1){
            for(int r=0;r<28;r++){
                int a = solMatrix[r][i];
                solM2[r][solM2Filled]=a;
            }
            solM2Filled++;
        }
    }
    

    for(int i=0;i<28;i++){
        solM2[i][27]=solMatrix[i][unknowns];
    }

    
    for(int i=0;i<unknowns;i++){
        if(doesNotContain(solMatrix[0][i],indexOfGuesses)==0){
            for(int r=0;r<28;r++){
                if(solMatrix[r][i]>0&&r>0){
                    solM2[r][solM2Filled+1]=-1;
                }else{
                    solM2[r][solM2Filled+1]=solMatrix[r][i];
                }
            }
            solM2Filled++;
        }
    }
    for(int i=solM2Filled+1;i<82;i++){
        for(int r=0;r<28;r++){
                solM2[r][solM2Filled]=solMatrix[r][i];
            
        }
        solM2Filled++;
     }
    

    
    printf("\n\n");
    for (int r = 0; r < 28; r++) {
        for (int c = 0; c < 82; c++) {
            printf("%d", solM2[r][c]);
            if (c < 82) {
                printf("%s", "|");
            }
        }
        printf("\n");
    }
    int reducable[27][28];
    int knownandGuess[27][54];

    for(int r=0;r<27;r++){
        for(int c = 0; c<28;c++){
            reducable[r][c]=solM2[r+1][c];
            printf("%d|", reducable[r][c]);
        }
        printf("\n");
    }

    for(int r=0;r<27;r++){
        for(int c = 28; c<82;c++){
            knownandGuess[r][c-28]=solM2[r+1][c];
            printf("%d|", knownandGuess[r][c]);
        }
        printf("\n");
    }

    


}

