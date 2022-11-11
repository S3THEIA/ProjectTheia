#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define LENGTH = 3
int main(int argc, char** argv)
{
    // Checks the number of arguments.
    /*
    arg0 : program's name
    arg1 : image's name
    */
    if (argc != 2)
        errx(EXIT_FAILURE, "need 1 argument\n
        image's name\n");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* sudokuCase = load_image("SudokuCase/%s(0,0).jpeg",argv[1]);//tester l'existance
    SDL_Surface* sudokuGrid = SDL_CreateRGBSurface(0,sudokuCase->w * LENGTH * LENGTH,sudokuCase->h * LENGTH * LENGTH,32,0,0,0,0); //creat a black image in function of the width and height of sudokucase


    //image cutting warning this function do not print delimitation between colone line or block.
    SDL_Rect spriteDst; // define position of each sudokuCase in sudokuGrid
    spriteDst.y = 0;
    for (int row = 0; row < LENGTH*LENGTH; row++)
    {
        spriteDst.x = 0;
        for (int col = 0; col < LENGTH*LENGTH; col++)
        {
            SDL_Surface* sudokuCase = load_image("SudokuCase/%s(%i,%i).jpeg",argv[1],row,col);
            SDL_BlitSurface(sudokuCase, NULL, sudokuGrid, &spriteDst);
            spriteDst.x += sudokuCase->h;//define the position of each case in sudokuGrid
        }
        spriteDst.y += sudokuCase->w; //define the position of each case in sudokuGrid
    }
    if (IMG_SaveJPG(sudokuGrid,  "SudokuCase.jpeg.result",100); != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());



    // Free the surface.
    SDL_FreeSurface(sudokuCase);
    SDL_FreeSurface(sudokuGrid);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}