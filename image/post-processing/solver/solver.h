#ifndef SOLVER_H
#define SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#endif
#define LENGTH 3


void check(int grid[][LENGTH*LENGTH],int res[], size_t line, size_t column);
int showgrid(int grid[][LENGTH*LENGTH]);
int is_solve(int grid[][LENGTH*LENGTH]);
int getsudoku(str filename, int sudoku[][]);
int solver(int grid[][LENGTH*LENGTH]);

