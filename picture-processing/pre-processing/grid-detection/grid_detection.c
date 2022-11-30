#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
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

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    r = average;
    g = average;
    b = average;
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}
//main fonction to detec the grid
//
//call recursively other function to forme the matrice of accumulation
//extract from the matrice of accumulation the grid
int detec_grid(SDL_Surface* surface)
{
    // Get the array of pixels.
     Uint32* pixels = surface->pixels;

     // Get the length of the array surface->w  surface->h;
    int result1[4];
    int result2[4];

    for (size_t row = 0; row < surface->w; row++)
    {
        for(size_t col = 0; col < surface->h; col++)
        {
            //go through each pixel
            //rajouter une optimisation pour pouvoir ignorer les pixels aui sont dans le precedent result1 et result2
            int tmp[4] = {row,col};
            for (int t = 0; t < 180;t++)
            {
                if(found_line(t,tmp))
                {
                    //on conserve l'ordre result1 > result2 > tmp
                }
                //parcourir tout les angles
            }
        }
    }
    return 1;
}

int founf_line(SDL_PixelFormat* format, int t, int* result)
{
    //detecte le nombre de case noire sur la ligne d'angle t et sous le point stocke dans result
    //lw parcour des ligne doit se faire en fonction de cos et sin
    //notament pour avoir le plus de ligne possible
    return 1;
}


int main(int argc, char *argv[])
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);

    SDL_FreeSurface(surface);

    // Destroy the objetcs.
    SDL_Quit();


    return EXIT_SUCCESS;
}
