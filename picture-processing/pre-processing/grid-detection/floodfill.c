#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../rotate-image/rotate.c"
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

void flood_pixel(Uint32* pixels, SDL_PixelFormat* format, size_t x, size_t y, size_t width, size_t height, int *coord)
{
    //amélioration ignore les gap dans une ligne
    
    Uint8 r, g, b;
    //apel sur les pixels adjacents
    if (y > 0)//pixel haut
    {
        SDL_GetRGB(pixel_color[x * width + y - 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {

            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[i] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, formqt, x, y - 1, width, height, coord);
        }
    }
    if (x < width)//pixel droit
    {
        SDL_GetRGB(pixel_color[(x+1) * width + y], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[i] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, formqt, x + 1, y, width, height, coord);
            if (x+1 < coord[2])
            {
                coord[2] = x+1;
                coord[3] = y;
            }
        }
    }
    
    if (y < height)//pixel bas
    {
        
        SDL_GetRGB(pixel_color[x * width + y + 1], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[i] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, formqt, x, y + 1, width, height, coord);
            if (y+1 < coord[4])
            {
                coord[4] = x;
                coord[5] = y+1;
            }
        }
    }
    if  (x > 0)//pixel gauche
    {
        SDL_GetRGB(pixel_color[(x-1) * width + y], format, &r, &g, &b);
        if (r < 10 && g < 10 && b < 10)
        {
            // change la couleur du pixel
            Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
            pixels[i] = SDL_MapRGB(format, done, done, done);
            flood_pixel(pixels, formqt, x - 1, y, width, height, coord);
            if (x-1 < coord[6])
            {
                coord[6] = x-1;
                coord[7] = y;
            }
        }
    }
}

int *flood_surface(SDL_Surface* surface)
{
    size_t *coord = malloc(sizeof(int) * 8);
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
    size_t len = surface->w * surface->h;

    // Lock the surface.
    int lock = SDL_LockSurface(surface);
    if (lock != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Convert each pixel into grayscale.
    SDL_PixelFormat* format = surface->format;
    Uint8 r, g, b;
    for (size_t x = 0; x < width; x++)
    {
        tmp[0] = x;
        for (size_t y = 0; y < height; y++)
        {
        
            SDL_GetRGB(pixel_color[x * width + y], format, &r, &g, &b);
            if (r < 10 && g < 10 && b < 10)
            {
                tmp[1] = y;
                flood_pixel(pixels, format, x, y, width, height, tmp);
                if (air(tmp) > air(coord))://air not define
                {
                    coord = tmp;
                }
            }
        }
    }

    // Unlock the surface.
    SDL_UnlockSurface(surface);
    // coord est le rectangle le plus grand soit le sudoku
    return realloc(coord,sizeof(int) * 4);
}


int main(int argc, char **argv)
{
    if (argc != 2)):
    {
        return 0;
    }
    surface *image = load_image(argv[1]);
    int coord[8] = flood_surface(image, x, v, coord);
    // coord == coordonnée des angles les plus écartés du sudoku

    //redimentionner l'image avec les coords
    //faire attention à ne pas seg fault
    //  haut gauche (coord[6] - 10,coord[1] - 10)
    //  haut droit (coord[2] + 10,coord[1] - 10)
    //  bas droit (coord[2] + 10,coord[5] + 10)
    //  bas gauche (coord[6] - 10,coord[5] + 10)



    //image cutting
    SDL_Rect spriteSrc; // define width and height of one sudoku
    spriteSrc.w = coord[2] - coord[6];
    spriteSrc.h = coord[5] - coord[1];


    if (coord[6] - 10 >= 0)
    {
        priteSrc.x = coord[6] - 10; //define the position of each case to copy
    }
    else
    {
        spriteSrc.x = 0;
    }
    if (coord[1] - 10 >= 0)
    {
        spriteSrc.y = coord[1] - 10; //define the position of each case to copy
    }
    else
    {
        spriteSrc.y = 0;
    }

    SDL_Surface* resize_Surface = SDL_CreateRGBSurface(0,spriteSrc.w,spriteSrc.h,0,0,0,0,0);
    SDL_BlitSurface(surface, &spriteSrc, resize_Surface, NULL);


    //call rotation with angle
    //rotation en fonction des coord bas
    //roation(image, (int) atan(  |(coord[5] - coord[7])/(coord[4] - coord[6])|)  ))
    //problème liée a l'orientation de l'image seront règlé plus tard

    
    SDL_Surface* rotated_surface = RotateSurface(resize_Surface, , (int) atan(  |(coord[5] - coord[7])/(coord[4] - coord[6])|));

    
    return 1;
}
