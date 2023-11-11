#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    arr_Generations[0].generation_size = arr_decision[2];
    int gen_index = 0;
    for(int i = 3;i<12; i++){
        if(arr_decision[i]==1){
            decision_copy_array(arr_decision[1],arr_decision[0],i-2,sud, arr_Generations[0].offspring[gen_index].generational_field);
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
            print(arr_Generations[0].offspring[gen_index].generational_field);
        }
    }
    printf("generation: 1, size: %d \n",arr_Generations[0].generation_size);

    int next_gen_size = 0;

    for(int i = 1;i<64;i++){

        gen_index = 0;

        for(int j = 0; j<arr_Generations[i-1].generation_size; j++){
            if(arr_Generations[i-1].offspring[i].dead!=1){
                decision_Array(arr_Generations[i-1].offspring[i].possibilities,arr_Generations[i-1].offspring[j].generational_field,arr_decision);
                next_gen_size += arr_decision[2];
            }
        }

        arr_Generations[i].offspring = (Generational_state *)malloc((next_gen_size+2) * sizeof(Generational_state));
        arr_Generations[i].generation_size = next_gen_size;


        for(int j = 0; j<arr_Generations[i-1].generation_size; j++){
            if(arr_Generations[i-1].offspring[j].dead!=1){
                decision_Array(arr_Generations[i-1].offspring[j].possibilities,arr_Generations[i-1].offspring[j].generational_field,arr_decision);
                if(j==3){
                    int a = 0;
                }
                for(int k = 3;k<12; k++){
                    if(arr_decision[k]==1){
                        int x = arr_decision[1];
                        int y = arr_decision[0];
                        printf("\n(%d,%d) = %d -> %d\n",x,y,arr_Generations[i-1].offspring[j].generational_field[y][x], k-2);
                        print(arr_Generations[i-1].offspring[j].generational_field);
                        decision_copy_array(arr_decision[1],arr_decision[0],k-2,arr_Generations[i-1].offspring[j].generational_field, arr_Generations[i].offspring[gen_index].generational_field);
                        fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        while(fill_conclusive(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field)) {
                            fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        }
                        fill_possibilities(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field);
                        if(!is_Valid(arr_Generations[i].offspring[gen_index].possibilities,arr_Generations[i].offspring[gen_index].generational_field)){
                            arr_Generations[i].offspring[gen_index].dead = 1;
                        }else if(is_solved(arr_Generations[i].offspring[gen_index].generational_field,sud)){
                            solved = 1;
                            break;
                        }
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
        
    }

    printf("The Solution:\n");
    print(sud);

    end_time = clock();
    printf("\n");
    printf("\n");
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n Generations Taken: %d\n", elapsed_time, generations);

    return 0;
}
