#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

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
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tempSurface,
            SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the temporary surface.
    SDL_FreeSurface(tempSurface);

    // Return the surface in the desired format.
    return surface;
}

double bilinearly_interpolate(unsigned int top, unsigned int bottom,
                              unsigned int left, unsigned int right,
                              double horizontal_position,
                              double vertical_position, SDL_Surface *surface)
{
    // Determine the values of the corners.
    Uint8 r, g, b;
    Uint32 *pixels = surface->pixels;
    printf("hello1\n");
    Uint32 tmp = pixels[left * surface->w  + top];
    printf("hello1\n");
    SDL_GetRGB(tmp,surface->format,&r,&g,&b);
    printf("hello1\n");
    double top_left = r;
    printf("hello2\n");

    tmp = pixels[right * surface->w  + top];
    SDL_GetRGB(tmp,surface->format,&r,&g,&b);
    double top_right = r;
    printf("hello3\n");

    tmp = pixels[left * surface->w  + bottom];
    SDL_GetRGB(tmp,surface->format,&r,&g,&b);
    double bottom_left = r;
    printf("hello4\n");

    tmp = pixels[right * surface->w  + bottom];
    SDL_GetRGB(tmp,surface->format,&r,&g,&b);
    double bottom_right = r;
    printf("hello5\n");
    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
        bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    return top_block + vertical_progress * (bottom_block - top_block);
}

SDL_Surface *resize(SDL_Surface *image, unsigned int newwidth, unsigned int newheight)
{
    unsigned int width = image->w;
    unsigned int height = image->h;

    double xscale = newwidth / (double)width;
    double yscale = newheight / (double)height;

     SDL_Surface *newimage = SDL_CreateRGBSurface(0, newwidth,newheight, image->format->BitsPerPixel,0, 0, 0, 0);
    printf("hey\n");
    Uint32 *pixels = newimage->pixels;
    for (unsigned int x = 0; x < newwidth; x++)
    {
        for (unsigned int y = 0; y < newheight; y++)
        {
            double oldx = x / xscale;
            double oldy = y / yscale;

            unsigned int top = floor(oldy);
            unsigned int bottom = top + 1;
            unsigned int left = floor(oldx);
            unsigned int right = left + 1;

            if (top < height && left < width && bottom < height
                && right < width)
            {
                printf("hey\n");
                double pixel = bilinearly_interpolate(
                    top, bottom, left, right, oldx, oldy, image);
                printf("hey\n");
                pixels[x * newwidth + y] = pixel;
            }
        }
    }

    return newimage;
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
        errx(EXIT_FAILURE, "need 6 arguments\n"
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
    SDL_Rect spriteSrc; // define width and height of one sudoku's case in
                        // advence
    spriteSrc.w = toint(argv[4])/(LENGTH*LENGTH);
    spriteSrc.h = toint(argv[5])/(LENGTH*LENGTH);
    spriteSrc.x = toint(argv[2]);
    spriteSrc.y = toint(argv[3]);
    char casename[] = "Sudoku00.jpeg";


    for (int row = 0; row < LENGTH*LENGTH; row++)
    {
        spriteSrc.x = toint(argv[2]);
        for (int col = 0; col < LENGTH*LENGTH; col++)
        {
            tempSurface = SDL_CreateRGBSurface(0, spriteSrc.w,spriteSrc.h, 32,
                    0, 0, 0, 0);

            if(SDL_BlitSurface(surface, &spriteSrc, tempSurface, NULL))
            {
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            }
            casename[6] = '0' + row;
            casename[7] = '0' +  col;
            printf("hey\n");
            resize(tempSurface,28,28);
            printf("hey\n");
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
