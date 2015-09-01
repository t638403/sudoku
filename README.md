# A brute force sudoku generator
First install `glib 2.0` and `gcc` and run following command:

    gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o sudoku sudoku.c
    
Or, if you have `pkg-config` installed, run this command:
    
    gcc $(pkg-config --cflags --libs glib-2.0) -o sudoku sudoku.c
   
## Usage
I have impleneted two kinds of sudokus

1. The normal sudoku
2. The [hypersudoku (nrc)](https://en.wikipedia.org/wiki/Sudoku#Hypersudoku)

To generate 1 normal sudoku

    $ sudoku
    
To generate 10 normal sudokus

    $ sudoku 10
    
To generate 10 hypersudokus

    $ sudoku 10 nrc
    
## Implementation
Searches recursively for a solution and after that it removes cells one at a time until the soduoko isn't solvable
anymore. Solvable means no more than one solution exists. 