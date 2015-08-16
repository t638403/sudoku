# A Sudoku solver and generator
First install `glib 2.0` and `gcc` and run following command:

    gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o sudoku sudoku.c
    
Or, if you have `pkg-config` installed, run this command:
    
    gcc $(pkg-config --cflags --libs glib-2.0) -o sudoku sudoku.c
   
   
# Todo
* Create commandline options to solve and generate with different configs 