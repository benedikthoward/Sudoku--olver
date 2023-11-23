#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SudokuStuff.c"

typedef struct Generational_state{
    unsigned char generational_field[9][9];
    unsigned char possibilities[9][9][9];
    unsigned char dead;
}Generational_state;

int main() {

    clock_t start_time, end_time;
    
    // Record the start time
    start_time = clock();
    int generations = 0;
    int arr_decision[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

    Generational_state **previous_generation;
    int previous_generation_size = 0;

    Generational_state **current_generation;
    int current_generation_size = 0;

    unsigned char possibilities [9][9][9];

    unsigned char sud[9][9] =  {
        {3, 5, 4, 0, 1, 0, 0, 7, 9},
        {0, 0, 0, 3, 5, 0, 0, 4, 0},
        {0, 0, 8, 6, 0, 4, 0, 5, 3},
        {7, 0, 0, 0, 0, 2, 3, 9, 0},
        {0, 0, 0, 5, 0, 7, 0, 6, 0},
        {0, 0, 0, 0, 0, 6, 7, 0, 0},
        {0, 0, 0, 8, 0, 3, 0, 0, 0},
        {5, 0, 0, 0, 0, 0, 8, 2, 6},
        {0, 0, 0, 0, 0, 0, 9, 0, 4}
    };

    printf("The problem:\n");
    print(sud);
    printf("\n");

    
    //fill_conclusive returns if it made a change based on a field that is actually known 
    fill_possibilities(possibilities,sud);
    while(fill_conclusive(possibilities,sud)) {
        fill_possibilities(possibilities,sud);
    }
    fill_possibilities(possibilities,sud);

    decision_Array(possibilities,sud,arr_decision);

    previous_generation = malloc(sizeof(Generational_state));
    previous_generation[0] = malloc(sizeof(Generational_state));

    previous_generation_size = 1;
    copy_array(sud,previous_generation[0]->generational_field );
    previous_generation[0]->dead = 0;
    fill_possibilities(previous_generation[0]->possibilities,previous_generation[0]->generational_field);

    //this is the limit of generations for a valid sudoku game
    for(int i = 1;i<64;i++){
        //resets the index for this generation
        generations = i;


        current_generation_size = 0;
        //figures out how many new fields will be generated
        for(int j = 0; j<previous_generation_size; j++){
            if(previous_generation[j]->dead!=1){
                decision_Array(previous_generation[j]->possibilities,previous_generation[0]->generational_field,arr_decision);
                current_generation_size += arr_decision[2];
            }
        }

        //allocates memmory to this generation
        current_generation = malloc((current_generation_size)*sizeof(Generational_state));
        if (current_generation == NULL) {
            fprintf(stderr, "\nMemory allocation failed for offspring\n");
            return 1;  // Exit if allocation fails
        }
        
        int gen_index = 0;
        //indexes through the fields of the preveous generation
        for(int j = 0; j<previous_generation_size; j++){
            
            //only carries the preeous generations offspring if it can still be solved
            if(previous_generation[j]->dead!=1){

                //generates the array that encodes the decison that has to be made [y,x,number of possibilites, n1,n2]
                decision_Array(previous_generation[j]->possibilities,previous_generation[j]->generational_field,arr_decision);
                
                //parses the decision array by reading binary encoded possible values based on index
                for(int k = 3;k<12; k++){
                    //index-2 -> the value that is represented if 1 the position can have that value in the field
                    if(arr_decision[k]==1){
                        current_generation[gen_index] = malloc(sizeof(Generational_state));

                        //copies the array from the preveous generation into the new generation with one option of the decision having been implemented
                        copy_array(previous_generation[j]->generational_field, current_generation[gen_index]->generational_field);
                        current_generation[gen_index]->generational_field[arr_decision[0]][arr_decision[1]] = k-2;
                        
                        //fills as much as is conclusive
                        fill_possibilities(current_generation[gen_index]->possibilities,current_generation[gen_index]->generational_field);
                        while(fill_conclusive(current_generation[gen_index]->possibilities,current_generation[gen_index]->generational_field)) {
                            fill_possibilities(current_generation[gen_index]->possibilities,current_generation[gen_index]->generational_field);
                        }
                        fill_possibilities(current_generation[gen_index]->possibilities,current_generation[gen_index]->generational_field);
                        
                        //checks for the new field still being solvable or being solved
                        if(!is_Valid(current_generation[gen_index]->possibilities,current_generation[gen_index]->generational_field)){
                            current_generation[gen_index]->dead = 1;

                        }else{
                            current_generation[gen_index]->dead = 0;

                        }
                        
                        if(is_solved(current_generation[gen_index]->generational_field,sud)){
                            free(previous_generation);
                            free(current_generation);
                            goto finish;
                        }
                        gen_index++;
                    }
                    
                }
            }
        }
        for(int j=0;j<previous_generation_size;j++){
            free(previous_generation[j]);
        }
        free(previous_generation);
        previous_generation = current_generation;
        previous_generation_size = current_generation_size;
        
    }

    finish:
    printf("\nThe Solution:\n");
    print(sud);

    end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\nGenerations Taken: %d\n", elapsed_time, generations);

    return 0;
}