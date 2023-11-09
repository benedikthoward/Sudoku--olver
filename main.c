#include <stdio.h>
#include <stdlib.h>
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
    fill_possibilities(possibilities,sud);
    while(fill_conclusive(possibilities,sud)) {
        fill_possibilities(possibilities,sud);
    }

    int *decision =  decision_Array(possibilities,sud);
    int solved = 0;
    Generation gen1;
    gen1.offspring = (Generational_state *)malloc(decision[2] * sizeof(Generational_state));
    gen1.generation_size = decision[2];
    for(int i = 0;i<gen1.generation_size; i++){
        decision_copy_array(decision[1],decision[0],decision[i+3],sud, gen1.offspring[i].generational_field);
        fill_possibilities(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field);
        while(fill_conclusive(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field)) {
            fill_possibilities(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field);
        }
        if(!is_Valid(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field)){
            gen1.offspring[i].dead = 1;
        }else if(is_solved(gen1.offspring[i].generational_field,sud)){
            solved = 1;
        }
    }
    int next_gen_size;
    while(solved ==0){

        Generation gen2;
        next_gen_size = 0;
        for(int i = 0; i<gen1.generation_size; i++){
            if(gen1.offspring[i].dead==0){
                next_gen_size += decision_Array(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field)[2];
            }
        }

        gen2.offspring = (Generational_state *)malloc(next_gen_size * sizeof(Generational_state));
        gen2.generation_size = next_gen_size;

        int gen2_index = 0;
        for(int i = 0; i<gen1.generation_size; i++){
            if(gen1.offspring[i].dead==0){
                int *decision =  decision_Array(gen1.offspring[i].possibilities,gen1.offspring[i].generational_field);
                
                for(int j = 0;i<decision[2]; j++){

                    decision_copy_array(decision[1],decision[0],decision[i+3],gen1.offspring[i].generational_field, gen2.offspring[i].generational_field);
                    fill_possibilities(gen2.offspring[gen2_index].possibilities,gen2.offspring[gen2_index].generational_field);
                    while(fill_conclusive(gen2.offspring[gen2_index].possibilities,gen2.offspring[gen2_index].generational_field)) {
                        fill_possibilities(gen2.offspring[gen2_index].possibilities,gen2.offspring[gen2_index].generational_field);
                    }
                    if(!is_Valid(gen2.offspring[gen2_index].possibilities,gen2.offspring[gen2_index].generational_field)){
                        gen2.offspring[gen2_index].dead = 1;
                    }else if(is_solved(gen2.offspring[gen2_index].generational_field,sud)){
                        solved = 1;
                        break;
                    }
                    gen2_index++;
                }
                if(solved==1){
                        break;
                }
            }
            if(solved==1){
                        break;
            }
        }
        free(&gen1);

        if(solved == 0){

            Generation gen1;
            next_gen_size = 0;
            for(int i = 0; i<gen2.generation_size; i++){
                if(gen2.offspring[i].dead==0){
                    next_gen_size += decision_Array(gen2.offspring[i].possibilities,gen2.offspring[i].generational_field)[2];
                }
            }

            gen1.offspring = (Generational_state *)malloc(next_gen_size * sizeof(Generational_state));
            gen1.generation_size = next_gen_size;

            int gen1_index = 0;
            for(int i = 0; i<gen2.generation_size; i++){
                if(gen2.offspring[i].dead==0){
                    int *decision =  decision_Array(gen2.offspring[i].possibilities,gen2.offspring[i].generational_field);
                    
                    for(int j = 0;i<decision[2]; j++){

                        decision_copy_array(decision[1],decision[0],decision[i+3],gen2.offspring[i].generational_field, gen1.offspring[i].generational_field);
                        fill_possibilities(gen1.offspring[gen1_index].possibilities,gen1.offspring[gen1_index].generational_field);
                        while(fill_conclusive(gen1.offspring[gen1_index].possibilities,gen1.offspring[gen1_index].generational_field)) {
                            fill_possibilities(gen1.offspring[gen1_index].possibilities,gen1.offspring[gen1_index].generational_field);
                        }
                        if(!is_Valid(gen1.offspring[gen1_index].possibilities,gen1.offspring[gen1_index].generational_field)){
                            gen1.offspring[gen1_index].dead = 1;
                        }else if(is_solved(gen1.offspring[gen1_index].generational_field,sud)){
                            solved = 1;
                            break;
                        }
                        gen1_index++;
                    }
                    if(solved==1){
                        break;
                    }
                }
                if(solved==1){
                        break;
                }
            }
            free(&gen2);
        }

    }
    return 0;
}