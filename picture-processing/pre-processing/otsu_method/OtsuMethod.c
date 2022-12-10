#include <err.h>
#include <SDL.h>
#include <SDL_image.h>

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
// path: Path of the image.
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

int mean(unsigned int *histo, int start, int end) 
{
 	return histo[end - start];
}

int sum(unsigned int *histo, int start, int end) 
{
  int sum = 0;
  for (int i = start; i < end; i++)
    sum += (int) histo[i];
  return sum;
}

void histo(unsigned int histo[256], unsigned len, Uint32* image,
        SDL_Surface* image_surface) 
{
  for (unsigned int i = 0; i < len; i++)
  {
      Uint8 r, g, b;
      Uint32 pixel = image[i];
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      histo[r] += 1;
  }
}

int otsu(unsigned int histo[256], unsigned w,
         unsigned h) 
{
  double final_thresh = -1.0;
  int final_t = -1;
  double mean_weight = 1.0 / (w * h);
  for (int t = 1; t < 255; t++) 
  {
    double wb = (double) sum(histo, 0, t) * mean_weight;
    double wf = (double) sum(histo, t, 255) * mean_weight;

    int mub = mean(histo, 0, t);
    int muf = mean(histo, t, 255);

    double value = wb * wf * (mub - muf);
    value *= value;
    if (value > final_thresh) 
    {
      final_thresh = value;
      final_t = t;
    }
  }
  return final_t;
}

int main(int argc, char** argv)
{
	if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

     // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the colored image.
    SDL_Surface* image_surface = load_image(argv[1]);
    unsigned int len = image_surface->w*image_surface->h;
    Uint32* image = image_surface->pixels;
    unsigned int h[256];
    histo(h, len, image, image_surface);

    int t = otsu(h, image_surface->w, image_surface->h);
    int lock = SDL_LockSurface(image_surface);
    if(lock != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (unsigned int i = 0; i < len; i++) {
      Uint32 pixel = image[i];
      
      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      Uint8 av = r < t ? 0 : 255; // Black and white
      pixel = SDL_MapRGB(image_surface->format, av, av, av);
      image[i] = pixel;
  }
int save = IMG_SaveJPG(image_surface, "otsu-picture.jpeg", 100);
    if (save != 0) 
        errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_UnlockSurface(image_surface);
  // Free the surface.
  SDL_FreeSurface(image_surface);

  // Destroy the objetcs.
  SDL_Quit();

  return EXIT_SUCCESS;

}
