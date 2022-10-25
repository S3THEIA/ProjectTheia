#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Simple network that can learn XOR
// Feartures : sigmoid activation function, stochastic gradient descent, and mean square error fuction

// Activation function and its derivative
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }
double softmax(double x, double sum, double max) {return exp(x-max) / sum; }
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

// int char2int(char *c)
// {
//     int res = 0;
//     for (int i = 0; *(c+i) != 0; i++)
//         res = res*10 + (*(c+i)-'0');
//     return res;
// }


#define numInputs 784
#define numHiddenNodes 10
#define numOutputs 10
#define numTrainingSets 4200

// Forward propagation
void feedForward(int i,double hiddenLayerBias[], double hiddenLayer[], double training_inputs[][numInputs], double hiddenWeights[][numHiddenNodes], double outputLayerBias[], double outputLayer[], double outputWeights[][numOutputs])
{
    // Compute hidden layer activation
    for (int j=0; j<numHiddenNodes; j++)
    {
        double activation = hiddenLayerBias[j];
        for (int k=0; k<numInputs; k++)
            activation += training_inputs[i][k] * hiddenWeights[k][j];
        hiddenLayer[j] = sigmoid(activation);
    }
    // Compute output layer activation
    for (int j=0; j<numOutputs; j++)
    {
        double activation = outputLayerBias[j];
        for (int k=0; k<numHiddenNodes; k++)
            activation += hiddenLayer[k] * outputWeights[k][j];
        outputLayer[j] = sigmoid(activation);
    }
}

// Update weight and threshold
void update(int i,const double lr,double outputLayerBias[],double deltaOutput[], double outputWeights[][numOutputs],double hiddenLayer[],double hiddenLayerBias[],double deltaHidden[],double hiddenWeights[][numHiddenNodes],double training_inputs[][numInputs])
{
    for (int j=0; j<numOutputs; j++)
    {
        outputLayerBias[j] += deltaOutput[j] * lr;
        for (int k=0; k<numHiddenNodes; k++)
            outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
    }
    // Apply change in hidden weights
    for (int j=0; j<numHiddenNodes; j++)
    {
        hiddenLayerBias[j] += deltaHidden[j] * lr;
        for(int k=0; k<numInputs; k++)
            hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
    }
}

// Backward propagation
void backPropagation(int i,const double lr,double training_outputs[][numOutputs], double outputLayer[],double outputWeights[][numOutputs], double hiddenLayer[],double outputLayerBias[],double hiddenLayerBias[],double hiddenWeights[][numHiddenNodes],double training_inputs[][numInputs])
{
    // Compute change in output weights
    double deltaOutput[numOutputs];
    for (int j=0; j<numOutputs; j++)
    {
        double errorOutput = (training_outputs[i][j] - outputLayer[j]);
        deltaOutput[j] = errorOutput * dSigmoid(outputLayer[j]);
    }
    // Compute change in hidden weights
    double deltaHidden[numHiddenNodes];
    for (int j=0; j<numHiddenNodes; j++)
    {
        double errorHidden = 0.0f;
        for(int k=0; k<numOutputs; k++)
            errorHidden += deltaOutput[k] * outputWeights[j][k];
        deltaHidden[j] = errorHidden * dSigmoid(hiddenLayer[j]);
    }
    update(i,lr,outputLayerBias,deltaOutput,outputWeights,hiddenLayer,hiddenLayerBias,deltaHidden,hiddenWeights,training_inputs);
}

int main (void)
{
    const double lr = 0.1f;
    
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];
    
    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];
    
    double training_inputs[numTrainingSets][numInputs];
    double training_outputs[numTrainingSets][numOutputs];


    FILE* fp = fopen("train.csv", "r");
    if (!fp)
    {
        printf("Ouverture fichier impossible !");
        exit(0);
    }
    char line[6952];
    for(int j=0; j<numTrainingSets; j++)
    {
        fgets(line, 42000*6952, fp);
        char *token;
        token = strtok(line,",");
        //training_outputs[j][0] = char2int(token);
        token = strtok(NULL,",");
        for (int i=0; token!= NULL;i++)
        {
            //training_inputs[j][i] = char2int(token);
            token = strtok(NULL,",");
        }
    }
    fclose(fp);
    printf("goooooo");
    
    for (int i=0; i<numInputs; i++) {
        for (int j=0; j<numHiddenNodes; j++) {
            hiddenWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numHiddenNodes; i++) {
        hiddenLayerBias[i] = init_weight();
        for (int j=0; j<numOutputs; j++) {
            outputWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numOutputs; i++) {
        outputLayerBias[i] = init_weight();
    }
    
    int trainingSetOrder[numTrainingSets] = {};
    for(int i=1; i<numTrainingSets;i++)
        trainingSetOrder[i] = i;
    
    int numberOfEpochs = 1;
    // Train the neural network for a number of epochs
    for(int epochs=0; epochs < numberOfEpochs; epochs++) {

        // As per SGD, shuffle the order of the training set
        //shuffle(trainingSetOrder,numTrainingSets);

        // Cycle through each of the training set elements
        for (int x=0; x<numTrainingSets; x++) {
            
            int i = trainingSetOrder[x];
            
            // Forward pass
            feedForward(i,hiddenLayerBias, hiddenLayer, training_inputs, hiddenWeights, outputLayerBias, outputLayer, outputWeights);
            
            // Print the results from forward pass
            printf ("Input:%g %g  Output:%g    Expected Output: %g\n",
                    training_inputs[i][0], training_inputs[i][1],
                    outputLayer[0], training_outputs[i][0]);

            // Backprop
            backPropagation(i,lr,training_outputs,outputLayer,outputWeights,hiddenLayer,outputLayerBias,hiddenLayerBias,hiddenWeights,training_inputs);

        }
    }
    
    // Print final weights after training
    fputs ("Final Hidden Weights\n[ ", stdout);
    for (int j=0; j<numHiddenNodes; j++) {
        fputs ("[ ", stdout);
        for(int k=0; k<numInputs; k++) {
            printf ("%f ", hiddenWeights[k][j]);
        }
        fputs ("] ", stdout);
    }
    
    fputs ("]\nFinal Hidden Biases\n[ ", stdout);
    for (int j=0; j<numHiddenNodes; j++) {
        printf ("%f ", hiddenLayerBias[j]);
    }

    fputs ("]\nFinal Output Weights", stdout);
    for (int j=0; j<numOutputs; j++) {
        fputs ("[ ", stdout);
        for (int k=0; k<numHiddenNodes; k++) {
            printf ("%f ", outputWeights[k][j]);
        }
        fputs ("]\n", stdout);
    }

    fputs ("Final Output Biases\n[ ", stdout);
    for (int j=0; j<numOutputs; j++) {
        printf ("%f ", outputLayerBias[j]);
        
    }
    
    fputs ("]\n", stdout);

    return 0;
}