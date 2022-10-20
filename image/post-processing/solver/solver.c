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
				if(test[i] == 0){return 0;}
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
                printf("row == %lu col == %lu\n",row,col);
                showgrid(grid);
				for(size_t i = 0; i < LENGTH*LENGTH; i++)
				{
					printf("    res[%lu] == %i \n",i,res[i]);
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
                if (grid[row][col] == 0){return 0;}
			}//tester si on a trouver une valeur a mettre
		}
	}
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
        if(row% 3 == 2){printf("\n");}

	}
	return 0;
}

int getsudoku(char* filename, int sudoku[][LENGTH*LENGTH])
{

    FILE* fichier = fopen(filename,"r");
    if (fichier != NULL)
    {
        char chaine[LENGTH*LENGTH+LENGTH] = "";
        size_t row = 0;
        int offset_row = 0;
        int offset_col = 0;
        while (fgets(chaine, LENGTH*LENGTH+LENGTH, fichier) != NULL)
        {
            offset_col = 0;
            if(chaine[0] != '\n' && chaine[1] != '\n')
            {
                for (size_t col  = 0; col < LENGTH*LENGTH+LENGTH-1; col++)
                {
                    printf("row == %lu col == %lu chaine == '%s'\n",row,col,chaine);
                    if(isdigit(chaine[col]))
                    {
                        printf("isdigit is True\n");
                        sudoku[row - offset_row][col - offset_col] = chaine[col] - 48;
                    }
                    else
                    {
                        switch (chaine[col])
                        {
                            case '.':
                                sudoku[row - offset_row][col - offset_col] = 0;
                                break;
                            case ' ':
                                offset_col += 1;
                                printf("parse ' ' offset_col == %i\n",offset_col);
                                break;
                            default:
                                printf("isdigit == %i\n",isdigit(chaine[col]));
                                printf("chaine[col] == %c\n",chaine[col]);
                                sudoku[row - offset_row][col - offset_col] = chaine[col] - 55;
                        }
                    }
                }
            }
            else
            {
                offset_row += 1;
                printf("offset_row == %i\n",offset_row);
            }
            //traitement de la line de sudoku
            row++;
            printf("\n");
            printf("\n");
        }
        fclose(fichier);
    }
    else
    {
         printf("%s is not readable",filename);
         return 1;
    }
    return 0;
}
int setsudoku(char* filename, int sudoku[][LENGTH*LENGTH])
{
    size_t tmplen = sizeof(filename);
    char* extention = "result";
    char resultname[tmplen + 6];
    size_t i = 0;
    while (i < tmplen - 1)
    {
        resultname[i] = filename[i];
        i++;
    }
    for (size_t j = 0; j < 7; j++)
    {
        resultname[i + j] = extention[j];
    }
    FILE* fichier = fopen(resultname, "w+");
    if (!fichier){return 1;}
    for(size_t row = 0; row < LENGTH*LENGTH; row++)
    {
        for(size_t col = 0; col < LENGTH*LENGTH; col++)
        {
            fprintf(fichier,"%i",sudoku[row][col]);
            if (col % LENGTH == 2){fprintf(fichier, " ");}
        }
        if (row < LENGTH*LENGTH - 1)
        {
            fprintf(fichier, "\n");
        }
        if (row % LENGTH == 2)
        {
            fprintf(fichier, "\n");
        }
    }
    fclose(fichier);
    return 0;
}



int main(int argc, char **argv)
{
    if ( argc != 2)
    {
        printf("argc == %i",argc);
        return 1;
    }
    int sudoku[LENGTH*LENGTH][LENGTH*LENGTH] = {{0}};
    showgrid(sudoku);
    getsudoku(argv[1],sudoku);
    showgrid(sudoku);
    showgrid(sudoku);
    solver(sudoku);
    setsudoku(argv[1], sudoku);
	/*need to conver the char* into int** */
	return 0;
}
