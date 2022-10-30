#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void surface_to_grayscale(SDL_Surface* surface)
{
    // Get the array of pixels.
    Uint32* pixels = surface->pixels;

    // Get the length of the array.
    int len = surface->w * surface->h;

    // Lock the surface.
    int lock = SDL_LockSurface(surface);
    if (lock != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Convert each pixel into grayscale.
    SDL_PixelFormat* format = surface->format;
    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    // Unlock the surface.
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);

    // Convert the surface into grayscale.
    surface_to_grayscale(surface);

    // Save grayscale picture
    int save = IMG_SaveJPG(surface, "gray-image.jpeg", 100);
    if (save != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the surface.
    SDL_FreeSurface(surface);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
