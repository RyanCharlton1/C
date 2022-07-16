#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define index(board, x, y) board[x + 7 * y]


void set_index(int *board, int x, int y, int n){
    board[x + 7 * y] = n;
}

void place(int *board, int col){
    int p1 = 0, p2 = 0;
    for(int i = 0; i < 42; i++){
        if(board[i] == 1)
            p1++;
        if(board[i] == 2)
            p2++;
    }

    int i = 6;
    while(index(board, col, i))
        i--;

    set_index(board, col, i, p1 > p2 ? 2 : 1);
}


int col_full(int *board, int col){
    for(int i = 6; index(board, col, i); i--)
        return 0;

    return 1;
}

int winner(int *board){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            if(index(board, i, j) == index(board, i + 1, j + 1)\
            && index(board, i + 1, j + 1) == index(board, i + 2, j + 2)\
            && index(board, i + 2, j + 2) == index(board, i + 3, j + 3))
                return index(board, i, j);
            
            if(index(board, i, j) == index(board, i + 1, j)\
            && index(board, i + 1, j) == index(board, i + 2, j)\
            && index(board, i + 2, j) == index(board, i + 3, j))
                return index(board, i, j);

            if(index(board, i, j) == index(board, i, j + 1)\
            && index(board, i, j + 1) == index(board, i, j + 2)\
            && index(board, i, j + 2) == index(board, i, j + 3))
                return index(board, i, j);

            if(index(board, i + 3, j) == index(board, i + 2, j + 1)\
            && index(board, i + 2, j + 1) == index(board, i + 1, j + 2)\
            && index(board, i + 1, j + 2) == index(board, i, j + 3))
                return index(board, i ,j);
        }
    
    return 0;
}

void print_board(int *board){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){
            printf("%d ", index(board, j, i));
        }
        putchar('\n');
    }
}

int hueristic(int *board){
    if(winner(board))
        if(winner(board) == 2)
            return -1;
        else 
            return 1;

    int h = 0;

    for(int i = 0; i < 7; i++){
        if(!col_full(board, i)){
            int *boardcpy = malloc(42 * sizeof(int));
            memcpy(boardcpy, board, 42 * sizeof(int));
            place(boardcpy, i);
            print_board(boardcpy);
            putchar('\n');
            h += hueristic(boardcpy);
            free(boardcpy);
        }
    }

    return h;
}

int* evaluate(int *board){
    int *results = calloc(7, sizeof(int));

    for(int i = 0; i < 7; i++){
        int *boardcpy = malloc(42 * sizeof(int));
        memcpy(boardcpy, board, 42 * sizeof(int));
        place(boardcpy, i);
        results[i] = hueristic(boardcpy);
        free(boardcpy);
    }
}

int main(){
    int *board = calloc(7 * 6, sizeof(int));

    int *moves = evaluate(board);
    for(int i = 0; i < 6; i++)
        printf("%d, ", moves[i]);
    putchar('\n');

    return 0;
}