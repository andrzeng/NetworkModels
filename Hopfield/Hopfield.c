#include <stdio.h>
#include <stdlib.h>

struct  HopfieldNet{
	int size; //Length of weight vector
	float** weights; //A 2d array containing the network's weights
};

//Load a Hopfield network from a file
struct HopfieldNet LoadHopfield(char* filename)
{
	struct HopfieldNet net;
	
	FILE* f = fopen(filename, "r");
	
	//Check to ensure the file exists
	if(f == NULL)
	{
		printf("Error: Cannot open %s. Please ensure the name is correct", filename);
		return;
	}
	
	//The first value in the file contains the side length of the weight matrix, which is a square matrix
	fscanf(f, "%d", &net.size);
	
	//Using that length, we initialize the weights of our Hopfield network
	net.weights = malloc(sizeof(float*) * net.size);
	for(int i = 0; i<net.size; ++i)
	{
		net.weights[i] = malloc(sizeof(float) * net.size);
	}
	
	//Now we scan the rest of the values in the file, which specify the connection signs and amplitudes between the network neurons
	float weight;
	for(int i = 0; i<net.size; ++i)
	{
		for(int x = 0; x<net.size; ++x)
		{
			fscanf(f, "%f", &weight);
			net.weights[i][x] = weight;
		}
	}

	fclose(f);
	return net;	
}

//Learn (i.e. "store") a pattern from a file to the network
void LearnPattern(struct HopfieldNet *net, char* filename, float lr)
{
	FILE* f = fopen(filename, "r");
	
	//Copy the pattern to a two-dimensional array
	int patternSize = 0;
	fscanf(f, "%d", &patternSize);
	int pattern[patternSize][patternSize];
	
	int state;
	for(int i = 0; i<patternSize; ++i)
	{
		for(int x = 0; x<patternSize; ++x)
		{
			fscanf(f, "%d", &state);
			pattern[i][x] = state;
		}
	}
	fclose(f);
	
	/*
	Based on that array, we update the weights of the network according to the rule presented in this paper: 
	https://www.pnas.org/doi/10.1073/pnas.79.8.2554
	*/
	
	for(int i = 0; i<patternSize*patternSize; ++i)
	{
		for(int x = 0; x<patternSize*patternSize; ++x)
		{
			(*net).weights[i][x] += (pattern[i/patternSize][i%patternSize]*pattern[x/patternSize][x%patternSize])*lr;
		}
	}
}

//Print weights of network
void PrintWeights(struct HopfieldNet net)
{
	for(int i = 0; i<net.size*net.size; ++i)	
	{
		printf("%f ", net.weights[i/net.size][i%net.size]);
		if((i+1)%net.size == 0)
		{
			printf("\n");
		}
	}
}

int main(int argc, char* argv[])
{
	struct HopfieldNet net = LoadHopfield("net.txt");
	
	printf("Weights before training: \n");
	PrintWeights(net);
	
	LearnPattern(&net, "mem1.txt", 0.01);
	printf("\nWeights after training: \n");
	PrintWeights(net);
}
