#include <err.h>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
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

void insertionSort(Uint8 arr[], int len)
{
    int i, key, j;
    for (i = 1; i < len; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* image_surface = load_image(argv[1]);
    int lock = SDL_LockSurface(image_surface);
    if(lock != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    unsigned int w =image_surface->w;
    unsigned int h =image_surface->h;
    unsigned int len = w*h;

    Uint32* image = image_surface->pixels;

    Uint8 R[25];
    Uint8 G[25];
    Uint8 B[25];


    SDL_Surface* res = SDL_CreateRGBSurface(0,w,h,
            image_surface->format->BitsPerPixel,0,0,0,0);
    lock = SDL_LockSurface(res);

    if(lock != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* pixels_res = res->pixels;
    int tmp;
    for(unsigned int i = 0; i < len; i++)
    {
        tmp = 0;
        if( i < len-(2*w) && i > 2*w && i%w > 2 && i%w < w-2)
        {
            for (int col = -2; col<3; col++ )
            {
                for (int row = -2; row<3; row++ )
                {
                    Uint32 pixel = image[i+col+(row*w)];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                    R[tmp] = r;
                    G[tmp] = g;
                    B[tmp] = b;
                    tmp += 1;
                }

            }
            insertionSort(R, 25);
            insertionSort(G, 25);
            insertionSort(B, 25);

            Uint32 pixel2 = SDL_MapRGB(res->format, R[13], G[13], B[13]);
            pixels_res[i] = pixel2;
        }
        else
        {
            pixels_res[i] = image[i];
        }
    }

    int save = IMG_SaveJPG(res, "median-picture.jpeg", 100);
    if (save != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_UnlockSurface(image_surface);
    SDL_UnlockSurface(res);
    // Free the surface.
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(res);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;

}
