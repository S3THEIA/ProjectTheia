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

    char *originCase = "original/case0.PNG";
    char *solvedCase = "solved/case0.PNG";

    // Create a surface from the colored image.
    SDL_Surface* sudokuCase = NULL;//load_image(casename);//tester l'existance
    SDL_Surface* sudokuGrid = SDL_CreateRGBSurface(0,sudokuCase->w * LENGTH * LENGTH,sudokuCase->h * LENGTH * LENGTH,32,0,0,0,0); //creat a black image in function of the width and height of sudokuc



    if (IMG_SaveJPG(sudokuGrid, "test",100) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

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
    SDL_FreeSurface(sudokuCase);
    SDL_FreeSurface(sudokuGrid);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
