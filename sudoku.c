#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SUDOKU_SIZE 81

static void s_print_region(char *region);
static void s_print_sudoku(char *sudoku);

static char* s_create_solution();
static char* s_create_sudoku_from_solution(char* solution);

static char s_check_region(char *region);
static char s_check_sudoku(char *sudoku);

static char s_find_first_zero_index(char* sudoku);
static char s_solve_sudoku(char *sudoku, int* max, char* solutions);

static char* s_dupl_sudoku(char* sudoku);
static char s_eq_sudoku(char* s1, char* s2);

static void s_get_row_values(char *sudoku, char y, char * restrict row);
static void s_get_col_values(char *sudoku, char x, char * restrict col);
static void s_get_block_values(char *sudoku, char b, char * restrict block);
static void s_get_block_values(char *sudoku, char b, char * restrict block);
static void s_get_nrc_block_values(char *sudoku, char b, char * restrict block);

static char s_get_row_index(char i);
static char s_get_col_index(char i);
static char s_get_block_index(char i);
static char s_get_nrc_block_index(char i);

static void s_get_cell_possibs(char* sudoku, char i, int *possible_values);

static int puzzleType = 0;
static int occupied = 0;
int main(int argc, char *argv[]) {

    srandom(time(NULL) * getpid());
    int nrOfPuzzles = 1;
    if( argc == 2 ) {
        if (sscanf (argv[1], "%i", &nrOfPuzzles)!=1) {
            if(strcmp(argv[1], "nrc") == 0) {
                puzzleType=1;
            }
            else {
                printf ("error - not an integer");
            }
        }
    } else  if( argc == 3) {
        if (sscanf (argv[1], "%i", &nrOfPuzzles)!=1) {
            printf ("error - not an integer");
        }
        if(strcmp(argv[2], "nrc") == 0) {
            puzzleType=1;
        }
    }

    for(int i=0;(i<nrOfPuzzles||nrOfPuzzles==-1);i++) {
        char* solution = s_create_solution();
        s_print_sudoku(solution);
        printf(":");

        char* sudoku = s_create_sudoku_from_solution(solution);
        s_print_sudoku(sudoku);
        printf("\n");
        free(solution);
        free(sudoku);
    }

    return 0;
}

static char* s_create_solution() {
    char empty_sudoku[SUDOKU_SIZE];
    memset(empty_sudoku, 0, SUDOKU_SIZE);
    int max = 1;
    char *solution = calloc(SUDOKU_SIZE, 1);
    s_solve_sudoku(empty_sudoku, &max, solution);
    return solution;
}

static int random_compare(const void *p1, const void *p2)
{
    int result = random();
    int median = 1073741823;
    if (result < median)
    {
        return -1;
    }
    else if (result == median)
    {
        return 0;
    }

    return 1;
}

static char* s_create_sudoku_from_solution(char* solution) {
    char* sudoku = s_dupl_sudoku(solution);
    int indexes[SUDOKU_SIZE];
    int valid_indexes[SUDOKU_SIZE];
    int number_valid_indexes = 0;

    char solutions[SUDOKU_SIZE*2];

    for(int i=0;i<SUDOKU_SIZE;i++) {
        indexes[i] = i;
    }

    qsort(indexes, SUDOKU_SIZE, sizeof(int), random_compare);

    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        int currentIndex = indexes[i];
        sudoku[currentIndex] = 0;

        int max = 2;
        s_solve_sudoku(sudoku, &max, solutions);
        if(max == 1) {
            valid_indexes[number_valid_indexes++] = currentIndex;
        }
        free(sudoku);
        sudoku = s_dupl_sudoku(solution);
        for(int j=0;j<number_valid_indexes;j++) {
            sudoku[valid_indexes[j]] = 0;
        }
        occupied = SUDOKU_SIZE - number_valid_indexes;
    }
    return sudoku;
}

static char s_find_first_zero_index(char* sudoku) {
    char *first_zero = memchr(sudoku, 0, SUDOKU_SIZE);
    if (!first_zero)
    {
        return -1;
    }

    return first_zero - sudoku;
}

static char s_solve_sudoku(char* sudoku, int* max, char* solutions) {
    char sudoku_is_valid = s_check_sudoku(sudoku);
    if(sudoku_is_valid) {
        memcpy(solutions + (SUDOKU_SIZE * (*max - 1)), sudoku, SUDOKU_SIZE);
        *max = *max - 1;
        if(*max == 0) {return 1;}
        return 0;
    }
    char i = s_find_first_zero_index(sudoku);
    char found_first_zero_index = (i + 1);
    if(found_first_zero_index) {
        int possible_values[10];
        s_get_cell_possibs(sudoku, i, possible_values);
        qsort(possible_values, 10, sizeof(int), random_compare);
        for (int j = 0; j < 10; ++j) {
            if (!possible_values[j]) {
                continue;
            }

            sudoku[i] = possible_values[j];
            char found = s_solve_sudoku(sudoku, max, solutions);
            if(found) {return 1;}
            sudoku[i] = 0;
        }
    }
    return 0;
}

static void s_get_cell_possibs(char* sudoku, char i, int *possible_values) {
    int result = 10;
    char region[9];
    for(int i=0;i<result;i++) {
        possible_values[i] = i;
    }

    s_get_row_values(sudoku, s_get_row_index(i), region);
    for(char i=0;i<9;i++) {
        possible_values[region[i]] = 0;
    }

    s_get_col_values(sudoku, s_get_col_index(i), region);
    for(char i=0;i<9;i++) {
        possible_values[region[i]] = 0;
    }

    s_get_block_values(sudoku, s_get_block_index(i), region);
    for(char i=0;i<9;i++) {
        possible_values[region[i]] = 0;
    }

    if(puzzleType == 1 && s_get_nrc_block_index(i) > -1) {
        s_get_nrc_block_values(sudoku, s_get_nrc_block_index(i), region);
        for(char i=0;i<9;i++) {
            possible_values[region[i]] = 0;
        }
    }
}

static void s_get_row_values(char *sudoku, char y, char * restrict row) {
    for(char i=0;i<9;i++) {
        row[i] = sudoku[9 * y + i];
    }
}

static void s_get_col_values(char *sudoku, char x, char * restrict col) {
    char ci = 0;
    for(char i=0;i<SUDOKU_SIZE;i+=9) {
        col[ci] = sudoku[i+x];
        ci++;
    }
}

static void s_get_block_values(char *sudoku, char b, char * restrict block) {
    char bi=0;
    char first_row = b - b%3;
    char first_col = b%3 * 3;
    for(char i=0;i<3;i++) {
        for(char j=0;j<3;j++) {
            block[bi] = sudoku[(first_row + i) * 9 + j + first_col];
            bi++;
        }
    }
}

static void s_get_nrc_block_values(char *sudoku, char b, char * restrict block) {
    char blocks[4][9] = {
        {10, 11, 12, 19, 20, 21, 28, 29, 30},
        {14, 15, 16, 23, 24, 25, 32, 33, 34},
        {46, 47, 48, 55, 56, 57, 64, 65, 66},
        {50, 51, 52, 59, 60, 61, 68, 69, 70}
    };
    for(char i=0;i<9;i++) {
        block[i] = sudoku[blocks[b][i]];
    }
}

static char s_check_region(char* region) {
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

static char s_check_sudoku(char *sudoku) {
    char region[9];
    for(char i=0;i<9;i++) {
        s_get_row_values(sudoku, i, region);
        if(!s_check_region(region)) {
            return 0;
        }

        s_get_col_values(sudoku, i, region);
        if(!s_check_region(region)) {
            return 0;
        }

        s_get_block_values(sudoku, i, region);
        if(!s_check_region(region)) {
            return 0;
        }

        if(puzzleType == 1 && i < 4) {
            s_get_nrc_block_values(sudoku, i, region);
            if(!s_check_region(region)) {
                return 0;
            }
        }

    }
    return 1;
}

static void s_print_region(char *region) {
    for(char i=0;i<9;i++) {
        bool isEmptyField = region[i] == 0;
        if(!isEmptyField) {
            printf("%d", region[i]);
        } else {
            printf(" ");
        }
    }
}

static void s_print_sudoku(char *sudoku) {
    for(char i=0;i<9;i++) {
        s_print_region(sudoku + (i * 9));
    }
}

static char s_get_row_index(char i) {
    return i / 9;
}
static char s_get_col_index(char i) {
    return (i - (s_get_row_index(i) * 9));
}
static char s_get_block_index(char i) {
    char ri = s_get_row_index(i);
    char ci = s_get_col_index(i);
    char bi = (3 * (ri / 3) + (ci / 3));
    return bi;
}

static char s_get_nrc_block_index(char i) {
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

static char* s_dupl_sudoku(char* sudoku) {
    char* dupl = malloc(SUDOKU_SIZE);
    memcpy(dupl, sudoku, SUDOKU_SIZE);
    return dupl;
}

static char s_eq_sudoku(char* s1, char* s2) {
    return memcmp(s1, s2, SUDOKU_SIZE) == 0;
}
