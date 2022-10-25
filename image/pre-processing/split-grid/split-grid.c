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
    arg2 : abscissa of the top-left corner of the sudoku in the image
    arg3 : ordinate of the top-left corner of the sudoku in the image
    arg4 : width of the sudoku
    arg5 : height of the sudoku
    */
    if (argc != 6)
        errx(EXIT_FAILURE, "need 6 argument\n
        program's name\n
        image's name\n
        abscissa of the top-left corner of the sudoku in the image\n
        ordinate of the top-left corner of the sudoku in the image\n
        width of the sudoku\n
        height of the sudoku\n");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    SDL_Surface* tempSurface = NULL;

    //image cutting
    SDL_Rect spriteSrc; // define width and height of one sudoku's case in advence
    spriteSrc.w = argv[4]/(LENGTH*LENGTH);
    spriteSrc.h = argv[5]/(LENGTH*LENGTH);
    for (int row = 0; row < LENGTH*LENGTH; row++)
    {
        for (int col = 0; col < LENGTH*LENGTH; col++)
        {
            tempSurface = SDL_CreateRGBSurface(0,argv[4],argv[5],0,0,0,0,0);//ne fonctionne pas doit créer un empty surface
            spriteSrc.x = row * argv[4]; //define the position of each case to copy
            spriteSrc.y = col * argv[5];
            SDL_BlitSurface(surface, &spriteSrc, tempSurface, NULL);
            if (IMG_SaveJPG(tempSurface,  "SudokuCase/%s(%i,%i).jpeg",argv[1],row,col, 100); != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
        }
    }




    // Free the surface.
    SDL_FreeSurface(surface);
    SDL_FreeSurface(tempSurface);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}