#ifndef M_PI

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define M_PI 3.1415926535897932

#endif

SDL_Surface* load_image(const char* path);
Uint8* ref(SDL_Surface *surface, int i, int j);
Uint32 get_pixel(SDL_Surface *surface, int i, int j);
void put_pixel(SDL_Surface * surface,int x,int y, Uint32 pixel);
SDL_Surface* RotateSurface(SDL_Surface* surface, double angleDegrees);
