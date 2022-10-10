#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>





#define PI 3.14159265458

struct  HopfieldNet{
	int size; //Side length of each square memory
	float** weights; //A 2d array containing the network's weights
};

struct HopfieldNet LoadHopfield(char* filename)
{
	struct HopfieldNet net;
	
	FILE* f = fopen(filename, "r");
	if(f == NULL)
	{
		printf("Error: Cannot open %s. Please ensure the name is correct", filename);
		return;
	}
	
	fscanf(f, "%d", &net.size);
	net.weights = malloc(sizeof(float*) * net.size);
	for(int i = 0; i<net.size; ++i)
	{
		net.weights[i] = malloc(sizeof(float) * net.size);
	}
	
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

void LearnPattern(struct HopfieldNet net, char* filename, float lr)
{
	FILE* f = fopen(filename, "r");
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
	
	
	
	
}


void help()
{
	printf("Parameters:\ninit [d] to initialize a new memory  \nrcon [filename] to reconstruct the partial memory stored in [filename]\n ");
	printf("Learn [file]");
}

int main(int argc, char* argv[])
{
	
	struct HopfieldNet net = LoadHopfield("test.txt");
	/*
	if(argc <= 1)
	{
		help();
	}
	printf(argv[1]);
	*/
	
	
}
