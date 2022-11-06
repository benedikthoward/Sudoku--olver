#include <stdio.h>
//sorts the index into what 3x3 box it is in
int box(int r, int c)
{
    int a = 0;
    int b = 0;
    if (r<3)
    {
        a=0;
    }else if (r<6)
    {
        a = 3;
    }else
    {
        a = 6;
    }
    if (c<3)
    {
        b=0;
    }else if (c<6)
    {
        b = 1;
    }else
    {
        b = 2;
    }
    return a+b;

}

void IndexToVector(int index, int *Xcoordinate, int *Ycoordinate)
{
    x = index%9;
    y = (index-(index%9))/9;
    *Xcoordinate = x;
    *Ycoordinate = y;
}



void main(){
    int sud[9][9]={{0,3,0,9,2,0,4,0,1},{0,1,0,3,0,4,0,0,0},{4,0,7,6,0,0,0,9,3},{0,0,0,0,0,0,0,6,5},{1,0,0,7,9,0,3,0,4},{0,7,0,0,4,0,2,1,9},{7,5,0,4,6,0,0,3,0},{3,0,6,0,5,1,0,4,0},{0,4,0,0,7,3,0,0,0}};
//get the user to input the sudoku problem


   
    //prints the sudoku problem
    for(int r=0;r<9;r++)
    {
        for (int c=0;c<9;c++)
        {
            printf("%d",sud[r][c]);
            if (c<8){
                printf("%s","|");
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    int solMatrix[27][82];
//fill the matrix with zeros
    for (int r=0;r<27;r++)
    {
        for (int c=0;c<82;c++)
        {
           solMatrix[r][c] =0;
        }

    }

// fill the "known column" with the maximum sum of a row, column, box
    for(int i=0;i<27;i++)
    {
        solMatrix[i][81]=45;
    }

//fills the equation matrix with ones for the unknowns and o for knowns. knowns also get subtractd from the total for the equation
    for (int r=0;r<9;r++)
    {
        for (int c=0;c<9;c++)
        {
            int val = sud[r][c];
            if(val==0)
            {
                //places the value into the row equation
                solMatrix[r][r*9+c]=1;
                //places the value into the column equation
                solMatrix[c+9][r*9+c]=1;
                //places the value into the bo equation
                solMatrix[18+box(r,c)][r*9+c]=1;

            }
            else
            {
                int temp;
                //places the value into the row equation
                temp = solMatrix[r][81];
                temp = temp-val;
                solMatrix[r][81] = temp;
                //places the value into the column equation
                temp = solMatrix[c+9][81];
                temp =temp-val;
                solMatrix[c+9][81] = temp;
                //places the value into the bo equation
                temp = solMatrix[18+box(r,c)][81];
                temp =temp-val;
                solMatrix[18+box(r,c)][81] = temp;

            }
        }
    }
    //prints the sudoku problem
    for (int r=0;r<27;r++)
    {
        for (int c=0;c<82;c++)
        {
            printf("%d",solMatrix[r][c]);
            if (c<81){
                printf("%s","|");
            }
        }
        printf("\n");
    }
    // counts the number of empty columns
    int col=0;
    int sum =0;
    //finds the number of 
    for (int c=0;c<82;c++)
    {
        sum = 0;
        for (int r=0;r<27;r++)
        {
            sum += solMatrix[r][c];
        }
        if(sum==0)
        {
            col++;
        }
    }
    printf("\n");
    printf("%d",col);
    printf("\n");
    //stores the indecies of columns with unknowns in a array
    int a = 82-col;
    int arrOfColIndexUnknown[81];
    col=0;
    colKnown = 0;
    for (int c=0;c<81;c++)
    {
        sum = 0;
        for (int r=0;r<27;r++)
        {
            sum += solMatrix[r][c];
        }
        if(sum>0&&sum<5)
        {
            arrOfColIndexUnknown[col] = c;
            col++;
        }else if(sum==0)
        {
            arrOfColIndexUnknown[a+colKnown] = c;
            colKnown++;
        }
        else
        {
            arrOfColIndexUnknown[a-1] = c;
        }
    }
    printf("\n\n");
    for(int r=0;r<82;r++)
    {
        printf("%d",arrOfColIndexUnknown[r]);
        printf("%s","|");
        
    }

    printf("\n\n");
    
    int reducable[28][82];
    printf("\n\n");
    //fills the matrix with only the unknown columns then the sum of the unknowns and then the vaules of the knonws for evey equation
    for(int c=0; c<a;c++)
    {
        reducable[0][c] = arrOfColIndexUnknown[c];
        for(int r = 0;r<27;r++)
        reducable[r+1][c] = solMatrix[r][arrOfColIndexUnknown[c]];
    }
    reducable[0][sizeof(arrOfColIndexUnknown)]=0;
    //fills the known values into the right column
    for(int i=0;i<27;i++)
    {
        reducable[i+1][a]=solMatrix[i][81];
    }
// prints the reducable matrix
    for(int r=0;r<28;r++)
    {
        for (int c=0;c<82;c++)
        {
            printf("%d",reducable[r][c]);
            if (c<sizeof(reducable[1])-1){
                printf("%s","|");
            }
        }
        printf("\n");
    }
}


