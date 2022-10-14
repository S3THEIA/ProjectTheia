#include "solver.h"

void check(int grid[][LENGTH*LENGTH],int res[], size_t line, size_t column)
{
	//check possible element to put in a case
	size_t start_r = (line/LENGTH)*LENGTH;
	size_t start_c = (column/LENGTH)*LENGTH;
	for (size_t row = start_r; row < start_r + LENGTH;row++)
	{
		if (row != line)
		{
			for (size_t col = start_c; col < start_c + LENGTH; col++)
			{
				if(col != column && grid[row][col] != 0)
				{
					res[grid[row][col] - 1] = 1;
				}
			}
		}
	}
	for(size_t i = 0; i < LENGTH*LENGTH; i++)
	{
		if(grid[i][column] != 0){res[grid[i][column] - 1] = 1;}
		if(grid[line][i] != 0){res[grid[line][i] - 1] = 1;} 
	}
}

int is_solve(int grid[][LENGTH*LENGTH])
{
	for (size_t row = 0; row < LENGTH*LENGTH;row++)
	{
		for (size_t col = 0; col < LENGTH*LENGTH; col++)
		{
			int test[LENGTH*LENGTH] = {0};
			check(grid, test, row, col);
			for (size_t i = 0; i < LENGTH*LENGTH; i++)
			{
				if(!test[i]){return 0;}
			}
		}
	}
	return 1;
}

int solver(int grid[][LENGTH*LENGTH])
{
	for(size_t row = 0; row < LENGTH*LENGTH; row++)
	{
		for(size_t col = 0; col < LENGTH*LENGTH; col++)
		{
			if(grid[row][col] == 0)
			{
				int res[LENGTH*LENGTH] = {0};
				check(grid, res, row, col);
				size_t i = 0;
				printf("row == %lu col ==  %lu\n",row,col);
				if (i == LENGTH*LENGTH){return 1;}
				for(size_t i = 0; i < LENGTH*LENGTH; i++)
				{
					printf("res[%lu] == %i \n",i,res[i]);
					if(res[i] == 0)
					{
						grid[row][col] = i + 1;
						if (solver(grid))
						{
							return 1;
						}
						grid[row][col] = 0;
					}
				}
			}
		}
	}
	printf("echec result : \n");
	return is_solve(grid);
}

int showgrid(int grid[][LENGTH*LENGTH])
{
	for(size_t row = 0; row < LENGTH*LENGTH; row++)
	{
		for(size_t col = 0; col < LENGTH*LENGTH; col++)
		{
			printf("%i",grid[row][col]);
			if (col % 3 == 2){printf(" ");}
		}
		printf("\n");
	}
	return 0;
}

int getsudoku(str filename, int sudoku[][])
{
    FILE* fichier = fopen(filename,"r");
    if (fichier != NULL)
    {
        char chaine[LENGTH*LENGTH+LENGTH] = "";
        size_t row = 0;
        while (fgets(chaine, LENGTH*LENGTH+LENGTH, fichier) != NULL)
        {
            int
            for (size_t col  = 0; col < LENGTH*LENGTH+LENGTH; col++)
            {
                switch (chaine[row][col])
                    case '.':
                        sudoku[col][col]
                    case ' ':
                        sudoku
                    case 'A':
                        sudoku
                    case 'B':
                        sudoku
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
            }
            //traitement de la line de sudoku

        }
        fgets(chaine, LENGTH*LENGTH+LENGTH, fichier);
        fclose(fichier);
    }
    else
    {
         printf("%s is not readable");
         return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if ( argc != 1)
    {
        return 1;
    }
	/*need to conver the char* into int** */
	return 0;
}
