#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h> 

int getArraySize(FILE*);
void *findMax(void *);

int fd[2];
int fs[2];

//struct of arguments to pass into pthreads
struct args{
	int* inputArray;
	int size;
	int start;
	int end;
	int pthreadNumber;
	FILE* fp2;
};


int main(int argc, char** argv){

	if(argc != 2){
		printf("Error: insufficient arguments\n");
		exit(0);
	}

	FILE* fp = fopen(argv[1],"r");
	FILE* fp2 = fopen("output.txt", "w");
	
	if(fp == NULL){
		printf("file does not exist\n");
		exit(0);
	}

	//gets array size from file
	int size = getArraySize(fp);
	rewind(fp);
	int tempNumber = 0;
	int i = 0;
	int* inputArray =malloc(size * sizeof(int));
	
	//populates array
	while(i < size){
		fscanf(fp, "%d", &tempNumber);
		inputArray[i] = tempNumber;
		i++;
	}
	

	int numOfThreads = 0;
	printf("How many threads?\n");
	scanf("%d", &numOfThreads);

	pipe(fd);
	pipe(fs);
	pthread_t pthread;
	int j = 0;
	while (j < numOfThreads){
		
		//allocate space for struct and then assign arguments
		struct args *inputArgs = (struct args*)malloc(sizeof(struct args));
		inputArgs->size = size/numOfThreads;
		inputArgs->start = j*(size/numOfThreads);
		inputArgs->end = j*(size/numOfThreads)+(size/numOfThreads);
		inputArgs->inputArray = inputArray;
		inputArgs->pthreadNumber = j;
		inputArgs->fp2 = fp2;

		//create pthread, specifying function to run and input arguments
		if(pthread_create(&pthread, NULL, findMax, (void *)inputArgs) != 0){
		  printf("Pthread creation error: Pthread %d\n", j);
		}
		
		j++;
	}
	
	pthread_join(pthread, NULL);
	
	//piping threadIDs and index which hidden keys are found
	//int index = 0;
	//int threadID = 0;
	for(int i = 0; i < 3; i++){
		int * outputArgs = malloc(sizeof(int)*2);
		read(fs[0], &outputArgs, sizeof(outputArgs));
		//read(fs[0], &threadID, sizeof(threadID));
		fprintf(fp2, "Hi I am pthread %d and I found a hidden key at A[%d]\n", outputArgs[0], outputArgs[1]);
	}

	//piping max 
	int threadMax = 0;
	int max = 0;
	for(int i = 0; i < numOfThreads; i++){
		read(fd[0],&threadMax, sizeof(threadMax));
		if(threadMax > max){
			max = threadMax;
		} 
	}
	fprintf(fp2, "Max = %d\n", max);
	return 0;
}

//returns array size from file
int getArraySize(FILE* fp){
	int counter = 0;
	int tempValue = 0;
	while(fscanf(fp, "%d", &tempValue) != EOF){
		counter++;
	}
	return counter;
}

//finds the max value in specified array
void *findMax(void *input){
	fprintf(((struct args*)input)->fp2, "I am pthread: %d\n",(((struct args*)input)->pthreadNumber));
	int max = 0;
	for(int i = (((struct args*)input)->start); i < (((struct args*)input)->end); i++){
		//found hidden key
		if(((struct args*)input)->inputArray[i] == -50){
			int* outputArgs = malloc(sizeof(int) *2);
			outputArgs[0]= ((struct args*)input)->pthreadNumber;
			outputArgs[1]= i;
			write(fs[1], &outputArgs, sizeof(outputArgs));
		}
		
		//update max
		if(((struct args*)input)->inputArray[i] > max){
			max = ((struct args*)input)->inputArray[i];
		}
	}
	
	write(fd[1], &max, sizeof(max));
	return input;		
}
