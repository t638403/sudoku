# A brute force sudoku generator
First install `gcc` or `clang` and run following command:

    cc -O2 -o sudoku sudoku.c

## Usage
I have implemented two kinds of sudokus

1. The normal sudoku
2. The [hypersudoku (nrc)](https://en.wikipedia.org/wiki/Sudoku#Hypersudoku)

To generate 1 normal sudoku

    $ ./sudoku

Result

    4 6 5 3 9 8 1 2 7
    1 9 8 2 6 7 5 4 3
    3 2 7 4 1 5 9 6 8
    2 8 3 9 7 6 4 1 5
    5 4 1 8 3 2 7 9 6
    6 7 9 1 5 4 3 8 2
    7 3 4 6 2 9 8 5 1
    9 5 2 7 8 1 6 3 4
    8 1 6 5 4 3 2 7 9

    4 0 0 0 0 8 1 2 0
    0 0 0 0 6 0 0 0 0
    3 2 0 0 0 0 0 0 0
    0 0 0 9 0 0 0 0 5
    5 0 0 0 0 2 0 0 6
    0 0 9 1 0 0 0 0 0
    0 3 0 0 0 0 0 0 0
    0 0 0 7 0 0 0 3 0
    8 0 6 0 4 3 0 0 9
    Occupied cells: 22

To generate 10 normal sudokus

    $ ./sudoku 10

To generate 1 hypersudokus

    $ ./sudoku nrc

To generate 10 hypersudokus

    $ ./sudoku 10 nrc

## Implementation
Searches recursively for a solution and after that it removes cells one at a time until the soduoko isn't solvable
anymore. Solvable means no more than one solution exists.
