#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define M_PI 3.1415926535897932

//--------------------------------------rotate image---------------------------
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888,
            0);
    SDL_FreeSurface(tmp);
    return surface;
}

Uint8* ref(SDL_Surface *surface, int i, int j)
{
    int bytesPerPixel = surface->format->BytesPerPixel;
    return (Uint8 *)surface->pixels + j * surface->pitch + i * bytesPerPixel;
}

Uint32 get_pixel(SDL_Surface *surface, int i, int j)
{
    Uint8 *p = ref(surface, i, j);
    switch(surface->format->BytesPerPixel) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
    }
}

void put_pixel(SDL_Surface * surface,int x,int y, Uint32 pixel)
{
    Uint8 *p = ref(surface, x, y);
    switch(surface->format->BytesPerPixel) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

// Use the rotation matrix on each pixel.
SDL_Surface* RotateSurface(SDL_Surface* surface, double angleDegrees)
{
    double angle = angleDegrees * (M_PI / 180.0);
    double cosinus = cos(angle);
    double sinus = sin(angle);

    int height = surface->h;
    int width = surface->w;

    int heightFinal = round(fabs(height * cosinus) + fabs(width * sinus)) + 1;
    int widthFinal = round(fabs(width * cosinus) + fabs(height * sinus)) + 1;

    SDL_Surface* surfaceRotated = SDL_CreateRGBSurface
        (0, widthFinal, heightFinal, 32, 0, 0, 0, 0);

    // Adds a background to the final image.
    Uint32 pixelColor = SDL_MapRGB(surfaceRotated->format, 255, 255, 255);
    SDL_FillRect(surfaceRotated, NULL, pixelColor);

    int centerHeight = round(((height + 1) / 2) - 1);
    int centerWidth = round(((width + 1) / 2) - 1);

    int centerHeightFinal = round(((heightFinal + 1) / 2) - 1);
    int centerWidthFinal = round(((widthFinal + 1) / 2) - 1);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int y = height - 1 - i - centerHeight;
            int x = width - 1 - j - centerWidth;

            int yFinal = round(-x * sinus + y * cosinus);
            int xFinal = round(x * cosinus + y * sinus);

            yFinal = centerHeightFinal - yFinal;
            xFinal = centerWidthFinal - xFinal;

            if ((0 <= xFinal && xFinal < widthFinal) && (0 <= yFinal
                && yFinal < heightFinal) && xFinal >= 0 && yFinal >= 0)
            {
                Uint32 pixel = get_pixel(surface, j, i);
                put_pixel(surfaceRotated,xFinal, yFinal, pixel);
            }
        }
    }

    return surfaceRotated;
}

//--------------------------------------grid detection-------------------------
void flood_pixel(Uint32* pixels, SDL_PixelFormat* format, size_t row, size_t col, size_t width, size_t height, size_t *coord)
{
    Uint8 r, g, b;
    //definir les coordonnees des coins
    if(row < coord[0] || (row == coord[0] && col < coord[1]) )
    {
        printf("up left\n");
        coord[0] = row;
        coord[1] = col;
    }
    else if(col < coord[7] || (col == coord[7] && row > coord[6]) )
    {
        printf("down left\n");
        coord[6] = row;
        coord[7] = col;
    }
    else if(col > coord[3] || (col == coord[3] && row < coord[2]) )
    {
        printf("up right\n");
        coord[2] = row;
        coord[3] = col;
    }
    else if(row > coord[4] || (row == coord[4] && col > coord[5]) )
    {
        printf("down right\n");
        coord[4] = row;
        coord[5] = col;
    }
    
    //apel sur les pixels proche
    size_t range = 5;
    for (size_t offset_row = -range; offset_row <= range; offset_row++)
    {
        for (size_t offset_col = -range; offset_col <= range; offset_col++)
        {
            if (0<= row + offset_row && row + offset_row < height && 0<= col + offset_col && col + offset_col < width)
            {
                printf("hey\n");
                SDL_GetRGB(pixels[(row + offset_row) * width + col + offset_col], format, &r, &g, &b);
                if (r <  20 && g < 20 && b < 20)
                {
                 Uint8 done = 11;//0.3*r + 0.59*g + 0.11*b;
                 pixels[(row + offset_row) * width + col + offset_col] = SDL_MapRGB(format, 255, done, done);
                 flood_pixel(pixels, format, row + offset_row, col + offset_col, width, height, coord);
                }
            }
        }
    }

}

int get_aire(size_t *coord)
{
    return abs((coord[4] - coord[0]) * (coord[3] - coord[7]));
}

void flood_surface(SDL_Surface* surface,size_t *coord)
{
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
    for (size_t row = 0; row < surface->h; row++)
    {
        for (size_t col = 0 ; col < surface->w; col++)
        {
            SDL_GetRGB(pixels[row * width + col], format, &r, &g, &b);
            if (r < 20 && g < 20 && b < 20)
            {
                tmp[0] = row;
                tmp[1] = col;

                tmp[2] = row;
                tmp[3] = col;

                tmp[4] = row;
                tmp[5] = col;

                tmp[6] = row;
                tmp[7] = col;
                
                flood_pixel(pixels, format, row, col, width, height, tmp);
                if (get_aire(tmp) > get_aire(coord)) //air not define
                {
                    for (size_t i = 0; i < 9; i++)
                    {
                        coord[i] = tmp[i];
                    }
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
    if (argc != 2)
    {
        return 0;
    }

    // Initialize the SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface *image = load_image(argv[1]);
    
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    size_t coord[8] = {0};
    flood_surface(image, coord);
    // coord == coordonnée des angles les plus écartés du sudoku
    printf("coord[0] = %li "
            "coord[1] = %li "
            "coord[2] = %li "
            "coord[3] = %li "
            "coord[4] = %li "
            "coord[5] = %li "
            "coord[6] = %li "
            "coord[7] = %li\n"
            ,coord[0],coord[1],coord[2],coord[3],coord[4],coord[5],coord[6],coord[7]);
    //to test
    if (IMG_SaveJPG(image,  "flood-image.jpg", 100) != 0)
            {
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            }

    //image cutting
    SDL_Rect spriteSrc; // define width and height of one sudoku

    if (coord[0] >= 10)
    {
        spriteSrc.x = coord[0] - 10;
    }
    else
    {
        spriteSrc.x = 0;
    }
    if (coord[7] >= 10)
    {
        spriteSrc.y = coord[7] - 10;
    }
    else
    {
        spriteSrc.y = 0;
    }

    if (coord[3] <= image->w - 10)
    {
        spriteSrc.w = coord[3] - coord[7] + 10;
    }
    else
    {
        spriteSrc.w = coord[3] - coord[7] ;
    }
    if (coord[4] <= image->h - 10)
    {
        spriteSrc.h = coord[4] - coord[0] + 10;
    }
    else
    {
        spriteSrc.h = coord[4] - coord[0];
    }
    SDL_Surface* resize_Surface = NULL;
    resize_Surface = SDL_CreateRGBSurface(0, spriteSrc.w, spriteSrc.h, 32,0, 0, 0, 0);
    if(resize_Surface == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    if(SDL_BlitSurface(image, &spriteSrc, resize_Surface, NULL))
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    if (IMG_SaveJPG(resize_Surface,  "resize_Surface.jpg", 100) != 0)
            {
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            }

    double tmp = coord[5] - coord[7];
    if (tmp == 0)
    {
        tmp = 1;
    }
    tmp = (coord[4] - coord[6])/tmp;
    if (tmp < 0)
    {
        tmp = - tmp;
    }
    SDL_Surface* rotated_surface = RotateSurface(resize_Surface, atan(tmp));
    
    size_t tmplen = strlen(argv[1]);
    char* extention = ".result.jpeg";
    char resultname[tmplen + 11];
    size_t i = 0;
    while (i < tmplen)
    {
        resultname[i] = argv[1][i];
        i++;
    }
    for (size_t j = 0; j < 12; j++)
    {
        resultname[i + j] = argv[1][j];
    }

    if(IMG_SavePNG(rotated_surface, resultname) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //if(IMG_SavePNG(resize_Surface, "rotate-image2.jpeg") != 0)
        //errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the surface.
    SDL_FreeSurface(image);

    // Destroy the objetcs.
    SDL_Quit();

    return 1;
}
