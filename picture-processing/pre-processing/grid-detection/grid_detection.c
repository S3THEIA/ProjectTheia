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

int detec_grid(SDL_Surface* surface, int num_rhos,int num_thetas,int threshold)
{
    size_t edge_height = (size_t) surface->h;
    size_t edge_weight = (size_t) surface->w;
    size_t edge_height_half = edge_height / 2;
    size_t edge_weight_half = edge_weight / 2;

    double d = sqrt((double) (edge_height*edge_height) + (edge_weight * edge_weight));
    double dtheta = 180 / num_thetas;
    double drhos = (2 * d) / num_rhos;

    double cos_thetas[180];
    double sin_thetas[180];
    for (int thetas = 0; thetas < 180; thetas++)
    {
        cos_thetas[thetas] = cos(thetas);
        sin_thetas[thetas] = sin(thetas);
    }

    int accumulator[edge_height * edge_weight];
    for (size_t row = 0; row < edge_weight;row++)
    {
        for(size_t col = 0; col < edge_height; col++)
        {
            if (edge_image[row][col] == 0)
            {
                edge_point = [row - edge_weight_half][col - edge_height_half];
                ys,yx;
                for (size_t theta_idx = 0; theta_idx < 180;theta_idx++)
                {
                    rho = edge_point[1] * cos_thetas[theta_idx] + edge_point[0] * cos_thetas[theta_idx];
                rho_idx = 
                }
            }
        }
    }
}



int main(int argc, char *argv[])
{
    // Checks the number of arguments.
    if (argc != 1)
        errx(EXIT_FAILURE, "Usage: image-file");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image("../../../pictures/image_01_easy.jpeg");
    

    SDL_FreeSurface(surface);

    // Destroy the objetcs.
    SDL_Quit();


    return EXIT_SUCCESS;
}
