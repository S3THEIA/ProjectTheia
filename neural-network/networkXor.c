#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

// Simple network that can learn XOR
// Feartures : sigmoid activation function, stochastic gradient descent, and mean square error fuction

// Activation function and its derivative
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }
// Activation function and its derivative
double init_weight() { return ((double)rand())/((double)RAND_MAX); }

// Shuffle the dataset
void shuffle(int *array, size_t len)
{
    if (len > 1)
    {
        size_t i;
        for (i = 0; i < len - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (len - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

#define numInputs 2
#define numHiddenNodes 2
#define numOutputs 1
#define numTrainingSets 4

int main (int argc, char* argv[]) 
{
    // Test if the for the arguments and for the opening of the the save file
    if(argc > 2)   
        errx(1, "Error: Too many arguments");

    FILE *fileToWrite;
    FILE *fileToRead;    

    const double lr = 0.1f;
    
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];
    
    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];
    

    double training_inputs[numTrainingSets][numInputs] = {{0.0f,0.0f},
                                                          {1.0f,0.0f},
                                                          {0.0f,1.0f},
                                                          {1.0f,1.0f}};
    double training_outputs[numTrainingSets][numOutputs] = {{0.0f},
                                                            {1.0f},
                                                            {1.0f},
                                                            {0.0f}};
    
    if((fileToRead = fopen("networkXorSave.txt", "r")) == NULL)
    {
        
        for (int i =0; i<numInputs; i++) 
        {
            for (int j=0; j<numHiddenNodes; j++) {
                hiddenWeights[i][j] = init_weight();
            }
        }
        for (int i=0; i<numHiddenNodes; i++) 
        {
            hiddenLayerBias[i] = init_weight();
            for (int j=0; j<numOutputs; j++) {
                outputWeights[i][j] = init_weight();
            }
        }
        for (int i=0; i<numOutputs; i++) 
        {
            outputLayerBias[i] = init_weight();
        }   
    }
    else
    {
        // Exctract Data from save File
        double arr[9] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

        int numExtraction;
        for(numExtraction = 0; numExtraction < 9; numExtraction++)
        {
            if( fscanf(fileToRead,"%lf",&(arr[numExtraction])) == 0)
            {
                errx(1, "Error: fscan() didn't manage to load the correctly the data to extract");
            }
        }
        numExtraction = 0;
        // Initialise Bias and Weights
        for (int i=0; i<numInputs; i++) 
        {
            for (int j=0; j<numHiddenNodes; j++) 
            {
                hiddenWeights[i][j] = sigmoid(arr[numExtraction]);
                numExtraction++;
            }
        }
        for (int i=0; i<numHiddenNodes; i++) 
        {
            hiddenLayerBias[i] = sigmoid(arr[numExtraction]);
            numExtraction++;
            for (int j=0; j<numOutputs; j++) 
            {
                outputWeights[i][j] = sigmoid(arr[numExtraction]);
                numExtraction++;
            }
        }
        for (int i=0; i<numOutputs; i++) 
        {
            outputLayerBias[i] = sigmoid(arr[numExtraction]);
            numExtraction++;
        }
    }

    int trainingSetOrder[] = {0,1,2,3};
    int numberOfEpochs = 100000;
    if(argc == 2)
    {
        numberOfEpochs = strtoul(argv[1], NULL, 10);
    }
    // Train the neural network for a number of epochs
    for(int epochs=0; epochs < numberOfEpochs; epochs++) {

        // As per SGD, shuffle the order of the training set
        shuffle(trainingSetOrder,numTrainingSets);

        // Cycle through each of the training set elements
        for (int x=0; x < numTrainingSets; x++) {
            
            int i = trainingSetOrder[x];
            
            // Forward pass
            
            // Compute hidden layer activation
            for (int j=0; j<numHiddenNodes; j++) {
                double activation = hiddenLayerBias[j];
                 for (int k=0; k<numInputs; k++) {
                    activation += training_inputs[i][k] * hiddenWeights[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }
            
            // Compute output layer activation
            for (int j=0; j<numOutputs; j++) {
                double activation = outputLayerBias[j];
                for (int k=0; k<numHiddenNodes; k++) {
                    activation += hiddenLayer[k] * outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }
            
            // Print the results from forward pass
            //printf ("Input:%g %g  Output:%g    Expected Output: %g\n",
            //        training_inputs[i][0], training_inputs[i][1],
            //        outputLayer[0], training_outputs[i][0]);


            // Backprop
            
            // Compute change in output weights
            double deltaOutput[numOutputs];
            for (int j=0; j<numOutputs; j++) {
                double errorOutput = (training_outputs[i][j] - outputLayer[j]);
                deltaOutput[j] = errorOutput * dSigmoid(outputLayer[j]);
            }
            
            // Compute change in hidden weights
            double deltaHidden[numHiddenNodes];
            for (int j=0; j<numHiddenNodes; j++) {
                double errorHidden = 0.0f;
                for(int k=0; k<numOutputs; k++) {
                    errorHidden += deltaOutput[k] * outputWeights[j][k];
                }
                deltaHidden[j] = errorHidden * dSigmoid(hiddenLayer[j]);
            }
            
            // Apply change in output weights
            for (int j=0; j<numOutputs; j++) {
                outputLayerBias[j] += deltaOutput[j] * lr;
                for (int k=0; k<numHiddenNodes; k++) {
                    outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
                }
            }
            
            // Apply change in hidden weights
            for (int j=0; j<numHiddenNodes; j++) {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for(int k=0; k<numInputs; k++) {
                    hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }
    int lastI = trainingSetOrder[numTrainingSets-1];
    
    printf ("Input:%g %g  Output:%g    Expected Output: %g\n\n",
                    training_inputs[lastI][0], training_inputs[lastI][1],
                    outputLayer[0], training_outputs[lastI][0]);
    
    fileToWrite = fopen("networkXorSave.txt", "w");

    // Print final weights after training and Save them
    fputs ("Final Hidden Weights\n[ ", stdout);
    for (int j=0; j<numHiddenNodes; j++) {
        fputs ("[ ", stdout);
        for(int k=0; k<numInputs; k++) {
            printf ("%f ", hiddenWeights[k][j]);
            fprintf(fileToWrite, "%f ", hiddenWeights[k][j]);
        }
        fputs ("] ", stdout);
    }
    
    fputs ("]\nFinal Hidden Biases\n[ ", stdout);
    for (int j=0; j<numHiddenNodes; j++) {
        printf ("%f ", hiddenLayerBias[j]);
        fprintf(fileToWrite, "%f ", hiddenLayerBias[j]);
    }

    fputs ("]\nFinal Output Weights", stdout);
    for (int j=0; j<numOutputs; j++) {
        fputs ("[ ", stdout);
        for (int k=0; k<numHiddenNodes; k++) {
            printf ("%f ", outputWeights[k][j]);
            fprintf(fileToWrite, "%f ", outputWeights[k][j]);
        }
        fputs ("]\n", stdout);
    }


    fputs ("Final Output Biases\n[ ", stdout);
    for (int j=0; j<numOutputs; j++) {
        printf ("%f ", outputLayerBias[j]);
        fprintf(fileToWrite, "%f\n", outputLayerBias[j]);
    }
    fputs ("]\n", stdout);

    fclose(fileToWrite);
    if (fileToRead != NULL)
    {
        fclose(fileToRead);
    }

    return 0;
}