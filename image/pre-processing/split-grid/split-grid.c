#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#define LENGTH = 3

int toint(char* string)
{
    int result = 0;
    size_t i = 0;
    while(string[i] != 0)
    {
        result = 10 * result + string[i] - 48;
        i++;
    }
    return result;
}

SDL_Surface* load_image(const char* path)
{
    // Load the image into a temporary surface.
    SDL_Surface* tempSurface = IMG_Load(path);
    if (tempSurface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a new surface in the desired format.
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGB888, 0);
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
    arg2 : abscissa of the top-left corner of the sudoku in the image
    arg3 : ordinate of the top-left corner of the sudoku in the image
    arg4 : width of the sudoku
    arg5 : height of the sudoku
    */

    int LENGTH = 3;
    if (argc != 6)
    {
        errx(EXIT_FAILURE, "need 6 argument\n"
        "program's name\n"
        "image's name\n"
        "abscissa of the top-left corner of the sudoku in the image\n"
        "ordinate of the top-left corner of the sudoku in the image\n"
        "width of the sudoku\n"
        "height of the sudoku");
    }

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    SDL_Surface* tempSurface = NULL;

    //image cutting
    SDL_Rect spriteSrc; // define width and height of one sudoku's case in advence
    spriteSrc.w = toint(argv[4])/(LENGTH*LENGTH);
    spriteSrc.h = toint(argv[5])/(LENGTH*LENGTH);
    spriteSrc.x = toint(argv[2]);
    spriteSrc.y = toint(argv[3]);
    char casename[] = {'S','u','d','o','k','u','(',' ',',',' ',')',0};
    //char* casename = "Sudokucase( , )";
    
    

    printf("hello%i,%i,%i,%i,",spriteSrc.w,spriteSrc.h,spriteSrc.x,spriteSrc.y);


    for (int row = 0; row < LENGTH*LENGTH; row++)
    {
        printf("row == %i\n",row);
        spriteSrc.x = toint(argv[2]);
        for (int col = 0; col < LENGTH*LENGTH; col++)
        {
            printf("    col == %i\n",col);
            tempSurface = SDL_CreateRGBSurface(0, spriteSrc.w,spriteSrc.h, 32, 0, 0, 0, 0);

            if(SDL_BlitSurface(surface, &spriteSrc, tempSurface, NULL))
            {
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            }
            casename[7] = '0' + row;
            casename[9] = '0' +  col;
            
            if (IMG_SaveJPG(tempSurface,  casename, 100) != 0)
            {
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            }
            spriteSrc.x += spriteSrc.w;
        }
        spriteSrc.y += spriteSrc.h;
    }



    // Free the surface.
    SDL_FreeSurface(surface);
    SDL_FreeSurface(tempSurface);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
