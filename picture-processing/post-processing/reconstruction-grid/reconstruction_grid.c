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
    //char *solvedCase = "solved/case0.PNG";

    // Create a surface from the colored image.
    char tmp[] = {'t','e','s','t','c','a','s','e','0',0};

    SDL_Surface *originCase[9];
    for (char i = 0; i < 9; i++)
    {
        printf("boucle for : %i\n",i);
        originCase_path[13] = 49 + i;
        printf("originCase_path%s\n",originCase_path);
        originCase[i] = load_image(originCase_path);
        printf("boucle for : %i\n",i);
        tmp[8] = 49 + i;
        printf("boucle for : %i\n",i);
        printf("tmp%s\n",tmp);
        if (IMG_SaveJPG(originCase[i], tmp,100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
        printf("boucle for : %i\n",i);
    }
    //SDL_Surface* sudokuCase = load_image("./origin/case1.png");//tester l'existance
    SDL_Surface* sudokuGrid = SDL_CreateRGBSurface(0,originCase[0]->w * LENGTH * LENGTH,originCase[0]->h * LENGTH * LENGTH,32,0,0,0,0); //creat a black image in function of the width and height of sudokuc



    //if (IMG_SaveJPG(sudokuGrid, "testgrid",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("image 1");
    //if (IMG_SaveJPG(sudokuCase, "testcase",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("image 2");

    //image cutting warning this function do not print delimitation between colone line or block.
    SDL_Rect spriteDst; // define position of each sudokuCase in sudokuGrid
    spriteDst.y = 0;
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

    // Free the surface.
    //SDL_FreeSurface(sudokuCase);
    SDL_FreeSurface(sudokuGrid);
    for (size_t i = 0; i < 9; i++)
        free(originCase[i]);


    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
