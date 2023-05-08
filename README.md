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

    147295863659183247238746195725418936964352718381679524472531689813967452596824371:          591    72  7 6       1 936 6     1  8    5   7  3 6  81 96 4  5   24   

To generate 10 normal sudokus

    $ ./sudoku 10

To generate 1 hypersudokus

    $ ./sudoku nrc

To generate 10 hypersudokus

    $ ./sudoku 10 nrc

To generate an endless stream

    $ ./sudoku -1

## Implementation
Searches recursively for a solution and after that it removes cells one at a time until the soduoko isn't solvable
anymore. Solvable means no more than one solution exists.
