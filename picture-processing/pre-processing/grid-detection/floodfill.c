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

void flood_pixel(Uint32* pixels, SDL_PixelFormat* format, size_t row, size_t col, size_t width, size_t height, size_t *coord)
{
    //amélioration ignore les gap dans une ligne
    //remplacer x et y par row et col
    //printf("x = %li y = %li\n",x,y);
    /*printf("coord[0] = %li "
            "coord[1] = %li "
            "coord[2] = %li "
            "coord[3] = %li "
            "coord[4] = %li "
            "coord[5] = %li "
            "coord[6] = %li "
            "coord[7] = %li\n"
            ,coord[0],coord[1],coord[1],coord[3],coord[4],coord[5],coord[6],coord[7]);*/
    Uint8 r, g, b;
    //definir les coordonnees des coins
    if(row < coord[0] || (row == coord[0] && col < coord[1]) )
    {
        coord[0] = row;
        coord[1] = col;
    }
    if(col < coord[7] || (col == coord[7] && row < coord[0]) )
    {
        coord[6] = row;
        coord[7] = col;
    }
    if(col > coord[3] || (col == coord[3] && row < coord[2]) )
    {
        coord[2] = row;
        coord[3] = col;
    }
    if(row > coord[4] || (row == coord[4] && col > coord[5]) )
    {
        coord[4] = row;
        coord[5] = col;
    }

    //apel sur les pixels adjacents
    if ( row > 0)//go up
    {
        SDL_GetRGB(pixels[(row - 1) * width + col], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {

            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[(row - 1) * width + col] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, row - 1, col, width, height, coord);
        }
    }
    if (col < width - 1)//go right
    {
        SDL_GetRGB(pixels[row * width + col + 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[row * width + col + 1] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, row, col + 1, width, height, coord);
        }
    }
    
    if (row < height - 1)//go down
    {
        
        SDL_GetRGB(pixels[(row + 1) * width + col], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[(row + 1) * width + col] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, row + 1, col, width, height, coord);
        }
    }
    if  (col > 0)//go left
    {
        SDL_GetRGB(pixels[row * width + col - 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[row * width + col - 1] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, format, row, col - 1, width, height, coord);
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
    for (size_t row = 0; row < 1; row++)
    {
        for (size_t col = 0; col < 1; col++)
        {
            SDL_GetRGB(pixels[row * width + col], format, &r, &g, &b);
            if (r < 10 && g < 10 && b < 10)
            {
                printf("x = %li y = %li\n",row,col);
                tmp[0] = row;
                tmp[1] = col;

                tmp[2] = row;
                tmp[3] = col;

                tmp[4] = row;
                tmp[5] = col;

                tmp[6] = row;
                tmp[7] = col;
                
                flood_pixel(pixels, format, row, col, width, height, tmp);
                printf("end : flood_pixel\n");
                if (get_air(tmp) > get_air(coord)) //air not define
                {
                    coord = tmp;
                }
            }
        }
    }
    // Unlock the surface.
    SDL_UnlockSurface(surface);
    printf("end : flood_surface\n");
    // coord est le rectangle le plus grand soit le sudoku
}


int main(int argc, char **argv)
{
    /*if (argc != 2)
    {
        return 0;
    }*/
    //SDL_Surface *image = load_image("../../../pictures/image_01_easy.jpeg");//argv[1]);
    SDL_Surface *image = load_image("../split-grid/Sudoku(0,0)");
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
    printf("test\n");


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
    printf("test\n");


    SDL_Surface* resize_Surface = SDL_CreateRGBSurface(0,spriteSrc.w,spriteSrc.h,0,0,0,0,0);
    SDL_BlitSurface(image, &spriteSrc, resize_Surface, NULL);


    printf("test3\n");

    //call rotation with angle
    //rotation en fonction des coord bas
    //roation(image, (int) atan(  |(coord[5] - coord[7])/(coord[4] - coord[6])|)  ))
    //problème liée a l'orientation de l'image seront règlé plus tard

    int tmp = (coord[5] - coord[7])*(coord[4] - coord[6]);
    if (tmp < 0)
    {
        tmp = - tmp;
    }
    printf("test\n");

    SDL_Surface* rotated_surface = RotateSurface(resize_Surface, (int) atan(tmp) );

    printf("test : end\n");

    
    return 1;
}
