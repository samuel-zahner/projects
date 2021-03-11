#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include<sys/wait.h>

int getArraySize(FILE*);
int findMax(int*, int, int, int, int,FILE*);
int forkProcess(int*, int**, int, int, int, int,FILE*);

int main(int argc, char** argv){

  if(argc != 2){
    printf("Error: insufficient arguments");
    exit(0);
  }

  FILE* fp = fopen(argv[1],"r");
	
  if(fp == NULL){
    printf("file does not exist");
    exit(0);
  }


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
  FILE* fp2 = fopen("output.txt","w");
       
  int numOfProcesses = 0;
  printf("How many processes?\n");
  scanf("%d", &numOfProcesses);
  //allocate memory for piping matrix
  int ** fd = malloc(numOfProcesses * sizeof(int*));
  for(int i = 0; i < numOfProcesses; i++){
    fd[i] = malloc(2*sizeof(int));
    pipe(fd[i]);
  }	
	
  
  int max = 0;
  int tempMax = 0;
  int processNumber = 1;
  int pid = fork();
  if(pid > 0){//parent
    wait(&pid);
    max = findMax(inputArray, size, 0, size/numOfProcesses, processNumber,fp2);
    close(fd[1][1]);
    read(fd[1][0], &tempMax, sizeof(tempMax));
    close(fd[1][0]);
    if (tempMax > max){
      max = tempMax;
    }
    fprintf(fp2,"Max: %d\n",max);
  }
  else if (pid == 0){//child
    processNumber++;
    //call recursion method
    forkProcess(inputArray, fd, processNumber, size,size/numOfProcesses, numOfProcesses,fp2);
    
  }
  else{//pid < 0
    printf("forking error: Process Number: %d\n", processNumber);
    exit(0);
  }
	return 0;	

}

//loops through file and returns the number of elements
int getArraySize(FILE* fp){
  int counter = 0;
  int tempValue = 0;
  while(fscanf(fp, "%d", &tempValue) != EOF){
    counter++;
  }
  return counter;
}

//loops through array and replaces current max if the current number is greater
//if current number == -50, prints that it found hidden key
int findMax(int* inputArray, int size, int startIndex, int endIndex,int  processNumber,FILE*fp2){
  int max = 0;
  for(int i = startIndex; i < endIndex; i++){
    if(inputArray[i] == -50){
      fprintf(fp2,"I am process %d and I have found the hidden key at A[%d]\n",processNumber, i);
    }
    
    if(inputArray[i] > max){
      max = inputArray[i];
    }
  }
	
  return max;
		
}
//recursion forking loop
int forkProcess(int* inputArray, int** fd, int processNumber, int size, int startIndex, int numOfProcesses,FILE*fp2){
  int max = 0;
  int tempMax = 0;
  int pid2 = fork();
  if(pid2 > 0){//parent
    wait(&pid2);
    fprintf(fp2,"Hi I am process %d and my parent is process %d.\n", processNumber, processNumber-1);
    max = findMax(inputArray, size, startIndex, (startIndex + size/numOfProcesses), processNumber,fp2);
		   
    close(fd[processNumber][1]);
    read(fd[processNumber][0], &tempMax, sizeof(tempMax));
    close(fd[processNumber][0]);
    if (max > tempMax){
      tempMax = max;
    }
		       
    close(fd[processNumber-1][0]);
    write(fd[processNumber-1][1], &tempMax, sizeof(tempMax));
    close(fd[processNumber-1][1]);
  }
  else if(pid2 == 0){//child
    processNumber++;
    startIndex = startIndex + size/numOfProcesses;
    if (processNumber != numOfProcesses){
      forkProcess(inputArray, fd, processNumber, size, startIndex,numOfProcesses,fp2);
    }
    if (processNumber == numOfProcesses){
      tempMax = findMax(inputArray, size, startIndex, size, processNumber,fp2);
      fprintf(fp2,"Hi I am process %d and my parent is process %d.\n", processNumber, (processNumber-1));
				
      close(fd[processNumber -1][0]);
      write(fd[processNumber-1][1], &tempMax, sizeof(tempMax));
      close(fd[processNumber-1][1]);
				
    }
  }
  else{//pid < 0
    printf("forking error Process Number: %d\n", processNumber);
    exit(0);
  }
  return 0;
}
