#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>

#include "include/mnist_file.h"
#include "include/neural_network.h"

#define STEPS 0
#define BATCH_SIZE 100 

/**
 * Downloaded from: http://yann.lecun.com/exdb/mnist/
 */
const char * train_images_file = "data/train-images-idx3-ubyte";
const char * train_labels_file = "data/train-labels-idx1-ubyte";
const char * test_images_file = "data/t10k-images-idx3-ubyte";
const char * test_labels_file = "data/t10k-labels-idx1-ubyte";
/**
 * Calculate the accuracy of the predictions of a neural network on a dataset.
 */

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888,
            0);
    SDL_FreeSurface(tmp);
    return surface;
}

int main(int argc, char *argv[])
{
    if (argc != 82)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    mnist_dataset_t * train_dataset, * test_dataset;
    mnist_dataset_t batch;
    neural_network_t network;
    float loss;
    int i, batches;

    // Read the datasets from the files
    train_dataset = mnist_get_dataset(train_images_file, train_labels_file);
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);

    // Initialise weights and biases with random values
    neural_network_import_weights(&network, "networkXorSave.txt");
    //neural_network_random_weights(&network);

    // Calculate how many batches (so we know when to wrap around)
    batches = train_dataset->size / BATCH_SIZE;

    for (i = 0; i < STEPS; i++) {
        // Initialise a new batch
        mnist_batch(train_dataset, &batch, 100, i % batches);

        // Run one step of gradient descent and calculate the loss
        loss = neural_network_training_step(&batch, &network, 0.5);

        // Calculate the accuracy using the whole test dataset
        //accuracy = calculate_accuracy(test_dataset, &network);
    }

    accuracy = calculate_accuracy(test_dataset, &network);
     uint8_t label;
     float precision;
    
    SDL_Surface *image_surface;
    unsigned int w,h,len;
    Uint32* image;
    int lock;
    uint8_t res[81];
    float precisions[81];

     for (size_t i = 0; i < 82; i++)
     {
        image_surface = load_image(argv[i]);
        lock = SDL_LockSurface(image_surface);
        if(lock != 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());
        image = image_surface->pixels;
        Uint32 pixel = image[i];
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        uint8_t r2 = (uint8_t) r;
        res[i] = neural_network_predict(r2, &network, &precision);
        precisions[i] = precision;
     }
    
    neural_network_save_weights(&network, "networkXorSave.txt");

    // Cleanup
    mnist_free_dataset(train_dataset);
    mnist_free_dataset(test_dataset);

    return 0;
}
