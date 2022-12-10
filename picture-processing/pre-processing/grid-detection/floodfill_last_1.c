#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define M_PI 3.1415926535897932

//--------------------------------------rotate image---------------------------

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
    //amélioration ignore les gap dans une ligne
    //remplacer x et y par row et col
    //printf("x = %li y = %li\n",x,y);
    /**/printf("coord[0] = %li "
            "coord[1] = %li "
            "coord[2] = %li "
            "coord[3] = %li "
            "coord[4] = %li "
            "coord[5] = %li "
            "coord[6] = %li "
            "coord[7] = %li\n"
            ,coord[0],coord[1],coord[2],coord[3],coord[4],coord[5],coord[6],coord[7]);
    Uint8 r, g, b;
    //definir les coordonnees des coins
    if(row < coord[0] || (row == coord[0] && col < coord[1]) )
    {
        printf("hey\n");
        coord[0] = row;
        coord[1] = col;
    }
    if(col < coord[7] || (col == coord[7] && row > coord[6]) )
    {
        printf("hey\n");
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
            pixels[(row - 1) * width + col] = SDL_MapRGB(format, 255, done, done);
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
            pixels[row * width + col + 1] = SDL_MapRGB(format, 255, done, done);
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
            pixels[(row + 1) * width + col] = SDL_MapRGB(format, 255, done, done);
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
            pixels[row * width + col - 1] = SDL_MapRGB(format, 255, done, done);
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
    for (size_t row = 1; row < 2; row++)
    {
        for (size_t col = 1; col < 2; col++)
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
    printf("hey\n");
    SDL_Surface *image = load_image("../../../pictures/image_01_easy.jpeg");//argv[1]);
    //SDL_Surface *image = load_image("../split-grid/Sudoku(0,0)");
    SDL_Surface* tempSurface = NULL;
    SDL_Rect spriteSrctmp;
    spriteSrctmp.w = 500;
    spriteSrctmp.h = 500;
    spriteSrctmp.x = 0;
    spriteSrctmp.y = 0;
    printf("hey\n");
    tempSurface = SDL_BlitSurface(image, &spriteSrctmp, tempSurface, NULL);
    printf("truc\n");
    printf("hey%i\n",tempSurface->w);
    
    size_t coord[8] = {0};
    flood_surface(tempSurface, coord);
    // coord == coordonnée des angles les plus écartés du sudoku

    //redimentionner l'image avec les coords
    //faire attention à ne pas seg fault
    //  haut gauche (coord[6] - 10,coord[1] - 10)
    //  haut droit (coord[2] + 10,coord[1] - 10)
    //  bas droit (coord[2] + 10,coord[5] + 10)
    //  bas gauche (coord[6] - 10,coord[5] + 10)

    if(IMG_SavePNG(image, "rotate-image.jpeg") != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

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
    printf("test\n");
    printf("%f\n",atan(tmp));
    //SDL_Surface* rotated_surface = RotateSurface(resize_Surface, atan(tmp) );

    printf("test : end\n");

    //if(IMG_SavePNG(resize_Surface, "rotate-image2.jpeg") != 0)
        //errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Free the surface.
    SDL_FreeSurface(image);

    // Destroy the objetcs.
    SDL_Quit();

    return 1;
}
