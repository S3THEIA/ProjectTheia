#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
SDL_Surface* load_image(const char* path)
{
    // Load the image into a temporary surface.
    SDL_Surface* tempSurface = IMG_Load(path);
    if (tempSurface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a new surface in the desired format.
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tempSurface,
            SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the temporary surface.
    SDL_FreeSurface(tempSurface);

    // Return the surface in the desired format.
    return surface;
}

void construct_image(char* originfilename, char* solvedfilename,SDL_Surface **originCase,SDL_Surface **solvedCase,SDL_Surface *sudokuGrid)
{
    FILE* originfichier = fopen(originfilename,"r");
    FILE* solvedfichier = fopen(solvedfilename,"r");
    printf("hey1\n");
    if (originfichier != NULL && solvedfichier != NULL)
    {
        printf("hey2\n");
        int LENGTH = 3;
        char originchaine[12 + 10] = "";//LENGTH*LENGTH+LENGTH +1== 13 for sudoku
        char solvedchaine[12 + 10] = "";
        int height = originCase[0]->h;
        int width = originCase[0]->w;
        SDL_Rect spriteDst;
        spriteDst.y = 0;
        while (fgets(originchaine, LENGTH*LENGTH+LENGTH+10, originfichier) != NULL && fgets(solvedchaine, LENGTH*LENGTH+LENGTH+10, solvedfichier) != NULL)
        {
            spriteDst.x = 0;
            printf("row begin %s\n",solvedchaine);
            if(originchaine[0] != '\n' && originchaine[1] != '\n')
            {
                printf("row no jump\n");
                for (int col  = 0; col < LENGTH*LENGTH+LENGTH-1; col++)
                {
                    if(isdigit(originchaine[col]))
                    {
                        if(SDL_BlitSurface(originCase[originchaine[col] - 49], NULL, sudokuGrid, &spriteDst))
                        {
                            errx(EXIT_FAILURE, "%s", SDL_GetError());
                        }
                        spriteDst.x += width;
                    }
                    else if (isdigit(solvedchaine[col]))
                    {
                        if(SDL_BlitSurface(solvedCase[solvedchaine[col] - 49], NULL, sudokuGrid, &spriteDst))
                        {
                            errx(EXIT_FAILURE, "%s", SDL_GetError());
                        }
                        spriteDst.x += width;
                    }
                    else
                    {
                        spriteDst.x += 10;
                    }
                }
                spriteDst.y += height;
            }
            else
            {
                printf("row jump'%s'\n",originchaine);
                spriteDst.y += 10;
            }
        }
        fclose(originfichier);
        fclose(solvedfichier);
    }
    else
    {
         printf("%s or %s is not readable",originfilename, solvedfilename);
    }
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    /*
    arg0 : program's name
    arg1 : image's name
    */
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "need 1 argument\n"
        "image's name\n");
    }

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    int LENGTH = 3;

    char originCase_path[] = {'.','/','o','r','i','g','i','n','/','c','a','s','e','0','.','p','n','g',0};
    char solvedCase_path[] = {'.','/','s','o','l','v','e','d','/','c','a','s','e','0','.','p','n','g',0};

    // Create a surface from the colored image.
    SDL_Surface *originCase[9];
    SDL_Surface *solvedCase[9];

    for (char i = 0; i < 9; i++)
    {
        //load each image once and for all
        originCase_path[13] = 49 + i;
        solvedCase_path[13] = 49 + i;
        originCase[i] = load_image(originCase_path);
        solvedCase[i] = load_image(solvedCase_path);
    }
    SDL_Surface* sudokuGrid = SDL_CreateRGBSurface(0,originCase[0]->w * LENGTH * LENGTH* LENGTH,originCase[0]->h * LENGTH * LENGTH * LENGTH,32,0,0,0,0); //creat a black image in function of the width and height of sudokuc
    construct_image("sudoku", "sudoku.result",originCase,solvedCase,sudokuGrid);
    

    //if (IMG_SaveJPG(sudokuGrid, "testgrid",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("image 1");
    //if (IMG_SaveJPG(sudokuCase, "testcase",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("image 2");

    //image cutting warning this function do not print delimitation between colone line or block.
    /*
    for (int row = 0; row < LENGTH*LENGTH; row++)
    {
        spriteDst.x = 0;
        for (int col = 0; col < LENGTH*LENGTH; col++)
        {
            casename[7] =  '0' + row;
            casename[9] =  '0' + col;
            SDL_Surface* sudokuCase = load_image(casename);
            SDL_BlitSurface(sudokuCase, NULL, sudokuGrid, &spriteDst);
            spriteDst.x += sudokuCase->w;//define the position of each case in sudokuGrid
        }
        spriteDst.y += sudokuCase->h; //define the position of each case in sudokuGrid
    }
    if (IMG_SaveJPG(sudokuGrid, "resultname.result",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
*/
    if (IMG_SaveJPG(sudokuGrid, "resultname.result",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Free the surface.
    //SDL_FreeSurface(sudokuCase);
    SDL_FreeSurface(sudokuGrid);
    for (size_t i = 0; i < 9; i++)
        free(originCase[i]);


    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}