#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../rotate-image/rotate.h"
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

void flood_pixel(Uint32* pixels, SDL_PixelFormat* format, size_t x, size_t y, size_t width, size_t height, size_t *coord)
{
    //amélioration ignore les gap dans une ligne
    //remplacer x et y par row et col
    //printf("x = %li y = %li\n",x,y);
    printf("coord[0] = %li "
            "coord[1] = %li "
            "coord[2] = %li "
            "coord[3] = %li "
            "coord[4] = %li "
            "coord[5] = %li "
            "coord[6] = %li "
            "coord[7] = %li\n"
            ,coord[0],coord[1],coord[1],coord[3],coord[4],coord[5],coord[6],coord[7]);
    Uint8 r, g, b;
    //apel sur les pixels adjacents
    if (y > 0)//pixel haut gauche
    {
        SDL_GetRGB(pixels[x * width + y - 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {

            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[x * width + y - 1] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, x, y - 1, width, height, coord);
        }
    }
    if (x < width - 1)//pixel haut droit
    {
        SDL_GetRGB(pixels[(x+1) * width + y], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[(x+1) * width + y] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, x + 1, y, width, height, coord);
            if (x+1 > coord[2] || (x+1 == coord[2] && y < coord[3]))
            {
                coord[2] = x+1;
                coord[3] = y;
            }
        }
    }
    
    if (y < height - 1)//pixel bas droit
    {
        
        SDL_GetRGB(pixels[x * width + y + 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[x * width + y + 1] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, x, y + 1, width, height, coord);
            if (x > coord[4] || (x > coord[4] && y+1 == coord[4]))
            {
                coord[4] = x;
                coord[5] = y+1;
            }
        }
    }
    if  (x > 0)//pixel bas gauche
    {
        SDL_GetRGB(pixels[(x-1) * width + y], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[(x-1) * width + y] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, x - 1, y, width, height, coord);
            if (x-1 < coord[6] || (x-1 == coord[6] && y > coord[7]))
            {
                coord[6] = x-1;
                coord[7] = y;
            }
        }
    }
}

int get_air(size_t *coord)
{
    return (coord[2] - coord[6]) * (coord[5] - coord[1]);
}

void flood_surface(SDL_Surface* surface,size_t *coord)
{
    //coord[0] == coin haut gauche x
    //coord[1] == coin haut gauche y
    //coord[2] == coin haut droit x
    //coord[3] == coin haut droit y
    //coord[4] == coin bas droit x
    //coord[5] == coin bas droit y
    //coord[6] == coin bas gauche x
    //coord[7] == coin bas gauche y

    //optenir les pcoin semble compliqué
    size_t tmp[8];
    // Get the array of pixels.
    Uint32* pixels = surface->pixels;

    // Get the length of the array.
    size_t width = surface->w;
    size_t height = surface->h;

    // Lock the surface.
    if (SDL_LockSurface(surface) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Convert each pixel into grayscale.
    SDL_PixelFormat* format = surface->format;
    Uint8 r, g, b;
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            SDL_GetRGB(pixels[x * width + y], format, &r, &g, &b);
            if (r < 10 && g < 10 && b < 10)
            {
                printf("x = %li y = %li\n",x,y);
                tmp[0] = x;
                tmp[1] = y;

                tmp[2] = x;
                tmp[3] = y;

                tmp[4] = x;
                tmp[5] = y;

                tmp[6] = x;
                tmp[7] = y;
                
                flood_pixel(pixels, format, x, y, width, height, tmp);
                if (get_air(tmp) > get_air(coord)) //air not define
                {
                    coord = tmp;
                }
            }
        }
    }

    // Unlock the surface.
    SDL_UnlockSurface(surface);
    // coord est le rectangle le plus grand soit le sudoku
}


int main(int argc, char **argv)
{
    /*if (argc != 2)
    {
        return 0;
    }*/
    SDL_Surface *image = load_image("../../../pictures/image_01_easy.jpeg");//argv[1]);
    size_t coord[8] = {0};
    flood_surface(image, coord);
    // coord == coordonnée des angles les plus écartés du sudoku

    //redimentionner l'image avec les coords
    //faire attention à ne pas seg fault
    //  haut gauche (coord[6] - 10,coord[1] - 10)
    //  haut droit (coord[2] + 10,coord[1] - 10)
    //  bas droit (coord[2] + 10,coord[5] + 10)
    //  bas gauche (coord[6] - 10,coord[5] + 10)



    //image cutting
    SDL_Rect spriteSrc; // define width and height of one sudoku
    spriteSrc.w = coord[5] - coord[1];
    spriteSrc.h = coord[2] - coord[6];

    if (coord[6] >= 10)
    {
        spriteSrc.x = coord[6] - 10; //define the position of each case to copy
    }
    else
    {
        spriteSrc.x = 0;
    }
    if (coord[1] >= 10)
    {
        spriteSrc.y = coord[1] - 10; //define the position of each case to copy
    }
    else
    {
        spriteSrc.y = 0;
    }

    if (coord[2] <= image->w - 10)
    {
        spriteSrc.w = coord[2] - coord[6] + 10;
    }
    else
    {
        spriteSrc.x = coord[2] - coord[6] ;
    }
    if (coord[5] <= image->h - 10)
    {
        spriteSrc.h = coord[5] - coord[1] + 10; //define the position of each case to copy
    }
    else
    {
        spriteSrc.h = image->h;
    }

    SDL_Surface* resize_Surface = SDL_CreateRGBSurface(0,spriteSrc.w,spriteSrc.h,0,0,0,0,0);
    SDL_BlitSurface(image, &spriteSrc, resize_Surface, NULL);


    //call rotation with angle
    //rotation en fonction des coord bas
    //roation(image, (int) atan(  |(coord[5] - coord[7])/(coord[4] - coord[6])|)  ))
    //problème liée a l'orientation de l'image seront règlé plus tard

    double tmp = (coord[5] - coord[7])/(coord[4] - coord[6]);
    if (tmp < 0)
    {
        tmp = - tmp;
    }
    SDL_Surface* rotated_surface = RotateSurface(resize_Surface, (int) atan(tmp) );

    
    return 1;
}
