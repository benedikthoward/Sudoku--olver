#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SudokuStuff.h"

typedef struct Generational_state{
    unsigned char generational_field[9][9];
    unsigned char possibilities[9][9][9];
    unsigned char dead;
}Generational_state;

typedef struct Generation{
    Generational_state *offspring;
    int generation_size;
}Generation;

int main() {

    clock_t start_time, end_time;
    
    // Record the start time
    start_time = clock();
    int generations = 0;
    int arr_decision[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    Generation* arr_Generations = (Generation*)malloc(64 * sizeof(Generation));

    unsigned char sud[9][9] =  {
        {0, 0, 0, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 4},
        {0, 0, 7, 0, 0, 3, 0, 9, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 9},
        {0, 0, 0, 2, 0, 0, 7, 0, 0},
        {0, 0, 9, 0, 5, 0, 0, 0, 6},
        {0, 0, 0, 0, 0, 6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    printf("The problem:\n");
    print(sud);

    unsigned char possibilities [9][9][9];
    //fill_conclusive returns if it made a change based on a field that is actually known 
    fill_possibilities(possibilities,sud);
    while(fill_conclusive(possibilities,sud)) {
        fill_possibilities(possibilities,sud);
    }
    fill_possibilities(possibilities,sud);

    decision_Array(possibilities,sud,arr_decision);
    int solved = 0;

    arr_Generations[0].offspring = (Generational_state *)malloc(arr_decision[2] * sizeof(Generational_state));
    memset(arr_Generations[0].offspring, 0, arr_decision[2] * sizeof(Generational_state));
    arr_Generations[0].generation_size = arr_decision[2];
    int gen_index = 0;
    for(int i = 3;i<12; i++){
        if(arr_decision[i]==1){
            copy_array(sud, arr_Generations[0].offspring[gen_index].generational_field);
            arr_Generations[0].offspring[gen_index].generational_field[arr_decision[0]][arr_decision[1]] = i-2;
            fill_possibilities(arr_Generations[0].offspring[gen_index].possibilities,arr_Generations[0].offspring[gen_index].generational_field);
            while(fill_conclusive(arr_Generations[0].offspring[gen_index].possibilities,arr_Generations[0].offspring[gen_index].generational_field)) {
                fill_possibilities(arr_Generations[0].offspring[gen_index].possibilities,arr_Generations[0].offspring[gen_index].generational_field);
            }
            fill_possibilities(arr_Generations[0].offspring[gen_index].possibilities,arr_Generations[0].offspring[gen_index].generational_field);
            if(!is_Valid(arr_Generations[0].offspring[gen_index].possibilities,arr_Generations[0].offspring[gen_index].generational_field)){
                arr_Generations[0].offspring[gen_index].dead = 1;
            }else if(is_solved(arr_Generations[0].offspring[gen_index].generational_field,sud)){
                solved = 1;
            }
            gen_index++;
        }
    }

    int next_gen_size = 0;
    int count_dead = 0;

    //this is the limit of generations for a valid sudoku game
    for(int i = 1;i<30;i++){
        //resets the index for this generation
        generations = i+1;
        gen_index = 0;
        next_gen_size = 0;

        

        //figures out how many new fields will be generated
        for(int j = 0; j<arr_Generations[i-1].generation_size; j++){
            if(arr_Generations[i-1].offspring[i].dead!=1){
                decision_Array(arr_Generations[i-1].offspring[i].possibilities,arr_Generations[i-1].offspring[j].generational_field,arr_decision);
                for(int k = 3;k<12; k++){
                    if(arr_decision[k]==1){
                        next_gen_size++;
                    }
                }
            }
        }
        if(generations ==11){
            int yes;
        }

        //allocates memmory to this generation
        arr_Generations[i].offspring = (Generational_state *)malloc((next_gen_size) * sizeof(Generational_state));
        if (arr_Generations[i].offspring == NULL) {
            fprintf(stderr, "\nMemory allocation failed for offspring\n");
            return 1;  // Exit if allocation fails
        }
        memset(arr_Generations[i].offspring, 0, next_gen_size * sizeof(Generational_state));
        arr_Generations[i].generation_size = next_gen_size;

        //indexes through the fields of the preveous generation
        for(int j = 0; j<arr_Generations[i-1].generation_size; j++){
            
            //only carries the preeous generations offspring if it can still be solved
            if(arr_Generations[i-1].offspring[j].dead!=1){

                //generates the array that encodes the decison that has to be made [y,x,number of possibilites, n1,n2]
                decision_Array(arr_Generations[i-1].offspring[j].possibilities,arr_Generations[i-1].offspring[j].generational_field,arr_decision);
                
                //parses the decision array by reading binary encoded possible values based on index
                for(int k = 3;k<12; k++){
                    //index-2 -> the value that is represented if 1 the position can have that value in the field
                    if(arr_decision[k]==1){

                        //indecies that will be filled for the next generation based on 
                        int x = arr_decision[1];
                        int y = arr_decision[0];

                        //prints the operation that is to be performed index goes from current val to new vale
                        printf("\n(%d,%d) -> %d\n",x,y,(k-2));
                        
                        //copies the array from the preveous generation into the new generation with one option of the decision having been implemented
                        copy_array(arr_Generations[i-1].offspring[j].generational_field, arr_Generations[i].offspring[gen_index].generational_field);
                        arr_Generations[i].offspring[gen_index].generational_field[arr_decision[0]][arr_decision[1]] = k-2;
                        
                        //fills as much as is conclusive
                        fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        while(fill_conclusive(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field)) {
                            fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        }
                        fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        
                        //checks for the new field still being solvable or being solved
                        if(!is_Valid(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field)){
                            arr_Generations[i].offspring[gen_index].dead = 1;
                            count_dead++;
                        }else if(is_solved(arr_Generations[i].offspring[gen_index].generational_field,sud)){
                            solved = 1;
                            break;
                        }
                        print(arr_Generations[i].offspring[gen_index].generational_field);
                        printf("\n ↑ %d",arr_Generations[i].offspring[gen_index].dead );
                        //increments the generation index counter
                        gen_index++;
                    }
                    if(solved==1){
                        break;
                    }
                }
                if(solved==1){
                        break;
                }
            }
            if(solved==1){
                        break;
            }
        }
        if(solved==1){
            break;
        }
        free(arr_Generations[i-1].offspring);
        printf("generation: %d, size: %d \n",generations,arr_Generations[i].generation_size);
        for(int j = 0; j<next_gen_size;j++){
            print(arr_Generations[i].offspring[j].generational_field);
            printf("\n ↑ %d",arr_Generations[i].offspring[j].dead );
        }
    }
    free(arr_Generations);

    printf("The Solution:\n");
    print(sud);

    end_time = clock();
    printf("\n");
    printf("\n");
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n Generations Taken: %d\n", elapsed_time, generations);

    return 0;
}