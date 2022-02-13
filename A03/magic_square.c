/*
 * Program Name : magic_square.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/10/2022
 * Identify whether a matrix is a magic square
 */

#include <stdio.h>
#include <stdlib.h>


//function to see if matrix is magic or not
void isMagic(int** matrix, int row, int col) {
    int digOne = 0, digTwo = 0;
    for (int i = 0; i < row; i++) {
        digOne += matrix[i][i];
        digTwo += matrix[i][row - 1 - i];
    }
    if (digOne != digTwo){
        printf("M is NOT a magic square!\n");
        return;
    }  
    int allRowSum = 0, allColSum = 0;
    for (int i = 0; i < row; i++) {
        int rowSum = 0, colSum = 0;
        for (int j = 0; j < col; j++) {
            rowSum = rowSum + matrix[i][j];
            colSum = colSum +  matrix[j][i];
        }
        if (rowSum != digOne || colSum != digOne || colSum != rowSum){
            printf("M is NOT a magic square!\n");
            return; 
        }
        if (i == 0){
            allRowSum = rowSum;
            allColSum = colSum;
        }
        else if (rowSum != allRowSum || colSum != allColSum) {
            printf("M is NOT a magic square!\n");
            return;
        }
    } 
    printf("M is a magic square (magic constant = %d)\n", allRowSum);
    printf("\n");
}

int main() {
    int row, col;
    scanf("%d %d", &row, &col);  

    int** matrix; //initializing a matrix 
    matrix = (int**)malloc(sizeof(int*)*row); //allocating the space for each of the rows and assigning it to matrix
    for (int i = 0; i < row; i++){
        matrix[i] = malloc(sizeof(int)*col); //allocating space for each column within each row
    }  
    //reading in the array
    for (int k = 0; k < row; k++) {
        for (int j = 0; j < col; j++) {
            int val = 0;
            scanf("%d", &val);
            matrix[k][j] = val;
        }
    }
    //printing out the array
    for (int k = 0; k < row; k++) {
        for (int j = 0; j < col; j++) {
            int num = matrix[k][j];
            printf(" %d", num);
        }
        printf("\n");
    }
    isMagic(matrix, row, col);
    
    for (int i = 0; i < row; i++){
        free(matrix[i]);
    } 
    free(matrix);
}

