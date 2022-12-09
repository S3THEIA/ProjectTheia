#include "rotate.h"

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

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file angle");

    // Initialize the SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the image.
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Convert the surface into tilt.
    SDL_Surface* surfaceFinal = RotateSurface(surface, atoi(argv[2]));

    if(SDL_SaveBMP(surfaceFinal, "rotate-image.jpeg") != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the surface.
    SDL_FreeSurface(surface);

    // Destroy the objetcs.
    SDL_Quit();

    return EXIT_SUCCESS;
}
