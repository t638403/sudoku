#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

void s_print_region(char *region);
void s_print_sudoku(char *sudoku);

char* s_create_solution();
char* s_create_sudoku_from_solution(char* solution);

char s_check_region(char *region);
char s_check_sudoku(char *sudoku);

char s_find_first_zero_index(char* sudoku);
char s_solve_sudoku(char *sudoku, int* max, GPtrArray* solutions);

char* s_dupl_sudoku(char* sudoku);
char s_eq_sudoku(char* s1, char* s2);

char* s_get_row_values(char *sudoku, char y);
char* s_get_col_values(char *sudoku, char x);
char* s_get_block_values(char *sudoku, char b);
char* s_get_nrc_block_values(char *sudoku, char b);

char s_divfloor(char n, char d);
char s_get_row_index(char i);
char s_get_col_index(char i);
char s_get_block_index(char i);
char s_get_nrc_block_index(char i);

GSList* s_get_cell_possibs(char* sudoku, char i);

static int puzzleType = 0;
static int occupied = 0;
char main(int argc, char *argv[]) {

    int nrOfPuzzles = 1;
    if( argc == 2 ) {
        if (sscanf (argv[1], "%i", &nrOfPuzzles)!=1) { printf ("error - not an integer"); }
    } else  if( argc == 3) {
        if (sscanf (argv[1], "%i", &nrOfPuzzles)!=1) { printf ("error - not an integer"); }
        if(strcmp(argv[2], "nrc") == 0) {puzzleType=1;}
    }

    for(int i=0;i<nrOfPuzzles;i++) {
        char* solution = s_create_solution();
        s_print_sudoku(solution);
        printf("\n");

        char* sudoku = s_create_sudoku_from_solution(solution);
        s_print_sudoku(sudoku);
        printf("Bezetting: %d\n", occupied);

//        int max = 8;
//        GPtrArray* solutions = g_ptr_array_new();
//        s_solve_sudoku(sudoku, &max, solutions);
//        printf("\n");
//        s_print_sudoku(g_ptr_array_index(solutions, 0));
//        printf("Solutions: %d\n", solutions->len);
    }
    

    
    return 0;
}

//    char sudoku[81] = {
//        1,9,0,6,0,0,0,2,3,
//        0,3,6,7,0,0,1,0,0,
//        0,0,0,1,0,0,0,0,5,
//        0,0,0,0,0,6,0,0,0,
//        0,0,4,8,0,0,0,9,0,
//        0,1,0,0,0,0,0,8,4,
//        0,0,0,0,0,8,4,0,0,
//        6,2,0,0,3,0,0,0,0,
//        9,0,3,0,0,0,2,0,0,
//    };

char* s_create_solution() {
    char* empty_sudoku = calloc(81, sizeof(char));
    for(char i=0;i<81;i++) {
        empty_sudoku[i] = 0;
    }
    int max = 1;
    GPtrArray* solutions = g_ptr_array_new();
    s_solve_sudoku(empty_sudoku, &max, solutions);
    return g_ptr_array_index(solutions, 0);
}


char* s_create_sudoku_from_solution(char* solution) {
    char* sudoku = s_dupl_sudoku(solution);

    GSList* all_indexes = NULL;
    for(int i=0;i<81;i++) {
        all_indexes = g_slist_prepend(all_indexes, GINT_TO_POINTER(i));
    }

    GSList* valid_indexes = NULL;
    while(g_slist_length(all_indexes) > 0) {
        int currentIndex = GPOINTER_TO_INT(g_slist_nth_data(all_indexes, g_random_int_range(0,g_slist_length(all_indexes))));
        all_indexes = g_slist_remove(all_indexes, GINT_TO_POINTER(currentIndex));
        sudoku[currentIndex] = 0;

        int max = 2;
        GPtrArray* solutions = g_ptr_array_new();

        s_solve_sudoku(sudoku, &max, solutions);
        if(solutions->len == 1) {
            valid_indexes = g_slist_prepend(valid_indexes, GINT_TO_POINTER(currentIndex));
        }
        g_ptr_array_free(solutions, TRUE);
        free(sudoku);
        sudoku = s_dupl_sudoku(solution);
        for(int i=0;i<g_slist_length(valid_indexes);i++) {
            int validIndex = GPOINTER_TO_INT(g_slist_nth_data(valid_indexes, i));
            sudoku[validIndex] = 0;
        }
        occupied = 81 - g_slist_length(valid_indexes);
    }
    return sudoku;
}

char s_find_first_zero_index(char* sudoku) {
    for(int i=0;i<81;i++) {
        if(sudoku[i] == 0) {
            return i;
        }
    }
    return -1;
}

char s_solve_sudoku(char* sudoku, int* max, GPtrArray* solutions) {
    char sudoku_is_valid = s_check_sudoku(sudoku);
    if(sudoku_is_valid) {
        g_ptr_array_add(solutions, s_dupl_sudoku(sudoku));
        *max = *max - 1;
        if(*max == 0) {return 1;}
        return 0;
    }
    char i = s_find_first_zero_index(sudoku);
    char found_first_zero_index = (i + 1);
    if(found_first_zero_index) {
        GSList* possibs = s_get_cell_possibs(sudoku, i);
        int length = g_slist_length(possibs);
        while(length>0) {
            sudoku[i] = (char)GPOINTER_TO_INT(g_slist_nth_data(possibs, g_random_int_range(0, length)));
            possibs = g_slist_remove(possibs, GINT_TO_POINTER(sudoku[i]));
            length = g_slist_length(possibs);
            char found = s_solve_sudoku(sudoku, max, solutions);
            if(found) {return 1;}
            sudoku[i] = 0;
        }
    }
    return 0;
}

GSList* s_get_cell_possibs(char* sudoku, char i) {
    GSList* possibs = NULL; 
    for(char i=1;i<10;i++) {
        possibs = g_slist_prepend(possibs, GINT_TO_POINTER(i));
    }

    char* row_values = s_get_row_values(sudoku, s_get_row_index(i));
    for(char i=0;i<9;i++) {
        possibs = g_slist_remove(possibs, GINT_TO_POINTER(row_values[i]));
    }
    free(row_values);

    char* col_values = s_get_col_values(sudoku, s_get_col_index(i));
    for(char i=0;i<9;i++) {
        possibs = g_slist_remove(possibs, GINT_TO_POINTER(col_values[i]));
    }
    free(col_values);

    char* block_values = s_get_block_values(sudoku, s_get_block_index(i));
    for(char i=0;i<9;i++) {
        possibs = g_slist_remove(possibs, GINT_TO_POINTER(block_values[i]));
    }
    free(block_values);

    if(puzzleType == 1 && s_get_nrc_block_index(i) > -1) {
        char* nrc_block_values = s_get_nrc_block_values(sudoku, s_get_nrc_block_index(i));
        for(char i=0;i<9;i++) {
            possibs = g_slist_remove(possibs, GINT_TO_POINTER(nrc_block_values[i]));
        }
        free(nrc_block_values);
    }
    return possibs;
}

char* s_get_row_values(char *sudoku, char y) {
    char* row = calloc(9, sizeof(char)); 
    for(char i=0;i<9;i++) {
        row[i] = sudoku[9 * y + i];
    }
    return row;
}

char* s_get_col_values(char *sudoku, char x) {
    char ci=0;
    char* col = calloc(9, sizeof(char));
    for(char i=0;i<81;i+=9) {
        col[ci] = sudoku[i+x];
        ci++;
    }
    return col;
}

char* s_get_block_values(char *sudoku, char b) {
    char bi=0;
    char first_row = b - b%3;
    char first_col = b%3 * 3;
    char* block = calloc(9, sizeof(char));
    for(char i=0;i<3;i++) {
        for(char j=0;j<3;j++) {
            block[bi] = sudoku[(first_row + i) * 9 + j + first_col];
            bi++;
        }
    }
    return block;
}

char* s_get_nrc_block_values(char *sudoku, char b) {
    char blocks[4][9] = {
        {10, 11, 12, 19, 20, 21, 28, 29, 30},
        {14, 15, 16, 23, 24, 25, 32, 33, 34},
        {46, 47, 48, 55, 56, 57, 64, 65, 66},
        {50, 51, 52, 59, 60, 61, 68, 69, 70}
    };
    char* block = calloc(9, sizeof(char));
    for(char i=0;i<9;i++) {
        block[i] = sudoku[blocks[b][i]];
    }
    return block;
}

char s_check_region(char* region) {
    char found[9] = {0,0,0,0,0,0,0,0,0};
    for(char i=0;i<9;i++) {
        if(region[i] == 0) {return 0;}
        found[region[i] - 1] = 1;
    }
    for(char i=0;i<9;i++) {
        if(found[i] == 0) {return 0;}
    }
    return 1;
}

char s_check_sudoku(char *sudoku) {
    for(char i=0;i<9;i++) {
        char* row = s_get_row_values(sudoku, i);
        char cr = s_check_region(row);
        if(cr == 0) {
            free(row);
            return 0;
        }
        char* col = s_get_col_values(sudoku, i);
        char cc = s_check_region(col);
        if(cc == 0) {
            free(col);
            return 0;
        }
        char* block = s_get_block_values(sudoku, i);
        char cb = s_check_region(block);
        if(cb == 0) {
            free(block);
            return 0;
        }
        if(puzzleType == 1 && i < 4) {
            char* nrc_block = s_get_nrc_block_values(sudoku, i);
            char cb = s_check_region(nrc_block);
            if(cb == 0) {
                free(nrc_block);
                return 0;
            }
            free(nrc_block);
        }
        free(row);
        free(col);
        free(block);
    }
    return 1;
}

void s_print_region(char *region) {
    for(char i=0;i<8;i++) {
        printf("%d ", region[i]);
    }
    printf("%d\n", region[8]);
}

void s_print_sudoku(char *sudoku) {
    for(char i=0;i<9;i++) {
        char* row = s_get_row_values(sudoku, i);
        s_print_region(row);
    }
}

char s_divfloor(char n, char d) {
    return ((n-(n%d))/d);
}
char s_get_row_index(char i) {
    return s_divfloor(i, 9);
}
char s_get_col_index(char i) {
    return (i - (s_get_row_index(i) * 9));
}
char s_get_block_index(char i) {
    char ri = s_get_row_index(i);
    char ci = s_get_col_index(i);
    char bi = (3 * s_divfloor(ri, 3) + s_divfloor(ci, 3));
    return bi; 
}

char s_get_nrc_block_index(char i) {
    char blocks[4][9] = {
        {10, 11, 12, 19, 20, 21, 28, 29, 30},
        {14, 15, 16, 23, 24, 25, 32, 33, 34},
        {46, 47, 48, 55, 56, 57, 64, 65, 66},
        {50, 51, 52, 59, 60, 61, 68, 69, 70}
    };

    for(char j=0;j<4;j++) {
        for(char k=0;k<9;k++) {
            if(i == blocks[j][k]) {
                return j;
            }
        }
    }
    return -1;
}

char* s_dupl_sudoku(char* sudoku) {
    char* dupl = calloc(81, sizeof(char));
    for(int i=0;i<81;i++) {
        dupl[i] = sudoku[i];
    }
    return dupl;
}

char s_eq_sudoku(char* s1, char* s2) {
    for(char i=0;i<81;i++) {
        if(s1[i] != s2[i]) {
            return 0;
        }
    }
    return 1;
}