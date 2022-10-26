#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_rotozoom.h> // { Lui ou l'un des deux suivant }
#include <SDL/SDL_gfx>
#include <SDL2/SDL2_gfx>

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file, angle (radian)");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the image.
    SDL_Surface* surface = IMG_Load(argv[1]);

    // { Possible utiliser qu'une surface ? }
    // Rotate image
    SDL_Surface* surfaceFinal = rotozoomSurface(surface, argv[2], 1.0, 1);

    // Save image
    int save = IMG_SaveJPG(surfaceFinal, "rotate-picture.jpeg", 100);
    if (save != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the surface.
    SDL_FreeSurface(surfaceFinal);
    SDL_FreeSurface(surface);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
