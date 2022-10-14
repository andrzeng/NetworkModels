#include <stdio.h>
#include <stdlib.h>

struct  HopfieldNet{
	int size; //side length of the weight matrix; equals the total number of cells in each memory
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
void ReconstructMemory(struct HopfieldNet net, char* input_file, char* output_file)
{
	
	FILE* f = fopen(input_file, "r");
	int patternSize = 0;
	fscanf(f, "%d", &patternSize);
	
	int partial_memory[patternSize][patternSize];
	
	int state;
	for(int i = 0; i<patternSize; ++i)
	{
		for(int x = 0; x<patternSize; ++x)
		{
			fscanf(f, "%d", &state);
			partial_memory[i][x] = state;
		}
	}
	fclose(f);
	
	float state_updates[patternSize][patternSize];
	
	
	for(int i = 0; i<patternSize; ++i)
	{
		for(int x = 0; x<patternSize; ++x)
		{
			//printf("%d",partial_memory[i][x]);
		}
		//printf("\n");
	}
	
	//printf("\n");
	
	
	bool quit = false;
	while(quit == false)
	{
		//Zero out array
		for(int i = 0; i<patternSize; ++i)
		{
			for(int x = 0; x<patternSize; ++x)
			{
				state_updates[i][x] = 0;
			}	
		}
		
		//Loop through each memory cell and sum up the activations of the Hopfield network
		for(int index = 0; index < patternSize*patternSize; ++index)
		{
			for(int index2 = 0; index2 < patternSize*patternSize; ++index2)
			{
				if(index != index2)
				{
					state_updates[index/patternSize][index%patternSize] += net.weights[index][index2] * partial_memory[index2/patternSize][index2%patternSize];
				}
			}
		}
		
		//printf("State updates: \n");
		for(int i = 0; i<patternSize; ++i)
		{
			for(int x = 0; x<patternSize; ++x)
			{
				//printf("%f  ",state_updates[i][x]);
			}
			//printf("\n");
		}
		
		
		//Update the partial memory
		//If the partial memory is not updated, the memory is marked as RECONSTRUCTED and will be returned as-is
		quit = true;
		for(int i = 0; i<patternSize; ++i)
		{
			for(int x = 0; x<patternSize; ++x)
			{
				if(state_updates[i][x] >= 0)
				{
					if(partial_memory[i][x] == -1)
						quit = false;
					partial_memory[i][x] = 1;
				}else{
					if(partial_memory[i][x] == 1)
						quit = false;
					partial_memory[i][x] = -1;
				}
			}	
		}
		
	}
	
	f = fopen(output_file, "w");
	for(int i = 0; i<patternSize; ++i)
	{
		for(int x = 0; x<patternSize; ++x)
		{
			fprintf(f, "%d ", partial_memory[i][x]);
			//printf("%d",partial_memory[i][x]);
		}
		fprintf(f,"\n");
		//printf("\n");
	}
	fclose(f);
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
	
	LearnPattern(&net, "mem1.txt", 0.01);
	LearnPattern(&net, "mem2.txt", 0.01);
	LearnPattern(&net, "mem3.txt", 0.01);
	LearnPattern(&net, "mem4.txt", 0.01);
	ReconstructMemory(net, "corruptmem1.txt", "reconstructed1.txt");
	ReconstructMemory(net, "corruptmem2.txt", "reconstructed2.txt");
	ReconstructMemory(net, "corruptmem3.txt", "reconstructed3.txt");
	ReconstructMemory(net, "corruptmem4.txt", "reconstructed4.txt");
	
}
