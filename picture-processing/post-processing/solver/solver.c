#include "solver.h"

void check(int grid[][LENGTH*LENGTH],int res[], size_t line, size_t column)
{
	// check possible element to put in a case
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
				for(size_t i = 0; i < LENGTH*LENGTH; i++)
				{
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
			}
		}
	}
	return is_solve(grid);
}

//int showgrid(int grid[][LENGTH*LENGTH])
//{
//	for(size_t row = 0; row < LENGTH*LENGTH; row++)
//	{
//		for(size_t col = 0; col < LENGTH*LENGTH; col++)
//  	{
//			printf("%i",grid[row][col]);
//			if (col % LENGTH == LENGTH - 1){printf(" ");}
//		}
//		printf("\n");
//        if(row% LENGTH  == LENGTH - 1){printf("\n");}

//	}
//	return 0;
//}

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
                    if(isdigit(chaine[col]))
                    {
                        sudoku[row - offset_row][col - offset_col] = chaine[col]
                            - 48;
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
                                break;
                            default:
                                sudoku[row - offset_row][col - offset_col] =
                                    chaine[col] - 55;
                        }
                    }
                }
            }
            else
            {
                offset_row += 1;
            }
            // processing of the sudoku line
            row++;
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

// write the final sudoku in filename.result
int setsudoku(char* filename, int sudoku[][LENGTH*LENGTH])
{
    size_t tmplen = strlen(filename);
    char* extention = ".result";
    char resultname[tmplen + 7];
    size_t i = 0;
    while (i < tmplen)
    {
        resultname[i] = filename[i];
        i++;
    }
    for (size_t j = 0; j < 8; j++)
    {
        resultname[i + j] = extention[j];
    }
    FILE* fichier = fopen(resultname, "w+");
    if (!fichier){return 1;}
    for(size_t row = 0; row < LENGTH*LENGTH; row++)
    {
        for(size_t col = 0; col < LENGTH*LENGTH; col++)
        {
            if(sudoku[row][col] < 10)
            {
                fprintf(fichier,"%i",sudoku[row][col]);
            }
            else
            {
                fprintf(fichier,"%c",sudoku[row][col]+7+'0');
            }
            if (col % LENGTH == LENGTH - 1){fprintf(fichier, " ");}
        }
        if (row < LENGTH*LENGTH - 1)
        {
            fprintf(fichier, "\n");
        }
        if (row % LENGTH == LENGTH - 1)
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
        errx(EXIT_FAILURE, "Usage: image-file");
    int sudoku[LENGTH*LENGTH][LENGTH*LENGTH] = {{0}};
    getsudoku(argv[1],sudoku);
    solver(sudoku);
    setsudoku(argv[1], sudoku);
	return 0;
}
