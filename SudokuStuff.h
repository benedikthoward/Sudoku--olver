
void fill_possibilities(unsigned char possibilities[9][9][9], unsigned char filled[9][9]){
    for(int i = 0; i<9;i++){
        for(int j = 0;j<9;j++){
            if(filled[i][j]==0){
                for(int k = 1;k<10;k++){
                    if(found_in_scope){
                        possibilities[i][j][k]=0;
                    }else{
                        possibilities[i][j][k]=1;
                    }
                }
            }
        }    
    }    
}

int found_in_scope(int i, int j, int looking_for, unsigned char filled[9][9]){
    for(int increm = 0; increm<9;increm++){
        if(filled[increm][j]==looking_for||filled[i][increm]==looking_for){
            return 1;
        }
    }
    
    int left = (i<3)? 0:(i<6) ? 3: 6;
    int top = (j<3)? 0:(j<6) ? 3: 6;

    for(int a = left; a<left+3;a++){
        for(int b = top; b< top+3; b++){
            if(filled[a][b]==looking_for){
                return 1;
            }
        }
    }
    return 0;
}

int fill_conclusive(unsigned char possibilities[9][9][9], unsigned char filled[9][9]){
    int boolean = 0;
    int sum;
    int index;
    for(int i = 0;i<9;i++){
        for(int j = 0; j<9;j++){
            if(filled[i][j]==0){
                sum = 0;
                for(int k =0 ;k<9;k++){
                    sum+=possibilities[i][j][k];
                    if(possibilities[i][j][k]==1){
                        index = k+1;
                    }
                }
                if(sum ==1){
                    filled[i][j] = k;
                    boolean =1;
                }
            }
        }    
    }
    return boolean;
}

//encodes a decision [index_Y, index_X, number_of_Outcomes, n1, n2, ...]
int* decision_Array(unsigned char possibilities[9][9][9],unsigned char sud[9][9]) {
    int min_outcomes = 2,147,483,647;
    int sum;
    int x;
    int y;
    for(unsigned char i = 0;i<9;i++){
        for(unsigned char j = 0; j<9;j++){
            sum = 0;
            if(sud[i][j]==0){
                for(unsigned char k =0 ;k<9;k++){
                    sum += possibilities[i][j][k];
                }
                if(sum<min_outcomes){
                    x = j;
                    y = i;
                    min_outcomes = sum;
                }
            }
        }
    }
    // Dynamically allocate an array of 'size' integers
    int* array = (int*)malloc((3+min_outcomes) * sizeof(int));

    array[0] = y
    array[1] = x;
    array[2] = min_outcomes;
    int preveous = 0;
    for (int i = 3; i < size; ++i) {
        for(int j = preveous;j<9;i++){
            if(possibilities[y][x][j]==1){
                array[i] = j+1;
                preveous = j+1;
                break;
            }
        }
    }
    
    // Return the pointer to the array
    return array;
}

int is_Valid(unsigned char possibilities[9][9][9],unsigned char sud[9][9]){
    for(unsigned char i = 0;i<9;i++){
        for(unsigned char j = 0; j<9;j++){
            sum = 0;
            if(sud[i][j]==0){
                for(unsigned char k =0 ;k<9;k++){
                    sum += possibilities[i][j][k];
                }
                if(sum==0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

 void decision_copy_array(int x, int y, int val, int sud[9][9], int into[9][9]) {
    
    // Copy the original array and change the specified value
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            into[i][j] = sud[i][j];
        }
    }

    into[x][y] = val;
}

int is_solved(unsigned char test[9][9], unsigned char final[9][9]){
    if(is_correct(test)){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                final[i][j] = test[i][j];
            }    
        }    
        return 1;
    }
    return 0;
}

int is_correct(unsigned char test[9[9]]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(test[i][j]==0){
                return 0;
            }
        }    
    }    
    return 1;
}