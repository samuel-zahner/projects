#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

struct variable{
	int value;
	char * name;
	int valid;
	int muxGrayCode;
};

int calculateGrayCode(int binary);
//int calculatePowerOfTwo(int number);
int andGate(int, int);
int orGate(int,int);
int notGate(int);
int xorGate(int, int);
int nandGate(int, int);
int norGate(int, int);
int xnorGate(int, int);
int multiplexer(int muxInputs, int selectors, struct variable* muxArray, struct variable* selectorArray);
int calculateBitsToDec(int numOfBits, struct variable* selectorArray);
struct variable* decoder(int decoderInputs, int decoderOutputs, struct variable* decoderInputArray, struct variable* decoderOutputArray);
int * initializeBoolArray(int* boolArray, int numOfLines);
bool checkBoolArray(int* boolArray, int numOfLines);

int main(int argc, char** argv){
	if (argc < 2){
		printf("insufficient arguments\n");
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	//FILE *fp2 = fopen("debug.txt", "w");
	if(fp == NULL)
	{
		printf("file does not exist\n");
		exit(0);
	}
	int numOfLines = 0;
	char* tempString = malloc(sizeof(char)* 50);
	while(fscanf(fp, "%s",tempString) != EOF){
		if(strcmp(tempString, "AND")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "OR")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "NOT")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "NAND")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "NOR")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "XOR")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "XNOR")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "DECODER")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "MULTIPLEXER")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "INPUTVAR")==0){
			numOfLines++;
		}
		else if(strcmp(tempString, "OUTPUTVAR")==0){
			numOfLines++;
		}
	}
	rewind(fp);
	int* boolArray = malloc(sizeof(int)*(numOfLines));
	boolArray = initializeBoolArray(boolArray, numOfLines);


	char* word = malloc(50 * sizeof(char));

	//Reading input vars
	fscanf(fp, "%s", word);
	if (strcmp(word, "INPUTVAR") != 0){
		printf("wrong file 1\n");
		exit(0);
	}
	//printf("a\n");
	int numOfInputs = 0;
	char* tempWord = malloc(50 * sizeof(char));
	fscanf(fp, "%d", &numOfInputs);
	char** nameArray = malloc(sizeof(char*)*numOfInputs);
	for(int i = 0; i< numOfInputs; i++){
		fscanf(fp, "%s", tempWord);
		char* new = malloc(50 * sizeof(char));
		nameArray[i] = strcpy(new,tempWord);
	}
	//printf("b\n");
	//reading output vars
	fscanf(fp, "%s", word);
	if(strcmp(word, "OUTPUTVAR") != 0){
		printf("wrong file 2\n");
		exit(0);
	}

	int numOfOutputs = 0;
	char* tempWord2 = malloc(50 * sizeof(char));
	fscanf(fp, "%d", &numOfOutputs);
	struct variable* outputArray = malloc(sizeof(struct variable)*numOfOutputs);

	for(int i = 0; i< numOfOutputs; i++){
		fscanf(fp, "%s", tempWord2);
		char* new = malloc(50 * sizeof(char));
		outputArray[i].name = strcpy(new,tempWord2);
	}
	rewind(fp);
	//printf("c\n");
	double counter = pow(2,numOfInputs);
	int grayCode = 0;
	struct variable* inputArray = malloc(sizeof(struct variable) * numOfInputs);
	for(int i = 0; i < counter; i++){
		struct variable* tempArray = malloc(sizeof(struct variable) * 50);
		grayCode = calculateGrayCode(i);
		for(int j = 0; j < numOfInputs; j++){
			struct variable temp;
			//int mask = calculatePowerOfTwo(j);
			temp.value = (grayCode >> (numOfInputs-1-j)) & 1;
			temp.name = nameArray[j];
			inputArray[j] = temp;
			printf("%d	", temp.value);
			//fprintf(fp2, "%d	", temp.value);
		}
		//printf("d\n");
		int recurseflag = 0;
		while(recurseflag == 0){
			int numOfLinesCounter=-1;
		while(fscanf(fp, "%s", word) != EOF){
			numOfLinesCounter++;
			//line already done
			if(boolArray[numOfLinesCounter] == 1){
				fgets(word, 150, fp);
				continue;
			}
			if(strcmp(word, "INPUTVAR")==0){
				fgets(word, 150, fp);
				boolArray[numOfLinesCounter] = 1;
			}else if(strcmp(word,"OUTPUTVAR")==0){
				fgets(word, 150, fp);
				boolArray[numOfLinesCounter] = 1;
			}

			else if(strcmp(word, "AND") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				//printf("e\n");
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				int inputCount = 0;
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 &&functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
//printf("f\n");
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
//printf("g\n");
				//one of variables missing - skip
				if(inputCount != 2){
					continue;
				}

				struct variable output;
				output.value = andGate(functionInput[0].value, functionInput[1].value);
				free(functionInput);
				output.name = output1;
				boolArray[numOfLinesCounter] = 1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						boolArray[numOfLinesCounter] = 1;
						flag = 1;
					}
				}
//printf("h\n");
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
		//printf("i\n");
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "OR") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}
					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 && functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 2){
					continue;
				}
				struct variable output;
				output.value = orGate(functionInput[0].value, functionInput[1].value);
				free(functionInput);
				output.name = output1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}

			}
			else if(strcmp(word, "NAND") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 &&functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 2){
					continue;
				}
				struct variable output;
				output.value = nandGate(functionInput[0].value, functionInput[1].value);
				free(functionInput);
				output.name = output1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "NOR") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 &&functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 2){
					continue;
				}
				struct variable output;
				output.value = norGate(functionInput[0].value, functionInput[1].value);
				free(functionInput);
				output.name = output1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "XOR") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 &&functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 2){
					continue;
				}
				struct variable output;
				output.value = xorGate(functionInput[0].value, functionInput[1].value);
				output.name = output1;
				free(functionInput);
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "XNOR") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* input2 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", input2);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 2);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid = 1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid = 1;
						inputCount++;
					}

					if(strcmp(input2, "1") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 1;
						functionInput[1].valid = 1;
						inputCount++;
					} else if(strcmp(input2, "0") == 0 && functionInput[1].valid != 1){
						functionInput[1].value = 0;
						functionInput[1].valid = 1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
					else if(strcmp(inputArray[i].name, input2) == 0 &&functionInput[1].valid != 1){
						functionInput[1] = inputArray[i];
						functionInput[1].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
						else if(strcmp(tempArray[i].name, input2) == 0){
							functionInput[1] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 2){
					continue;
				}
				struct variable output;
				output.value = xnorGate(functionInput[0].value, functionInput[1].value);
				free(functionInput);
				output.name = output1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "NOT") == 0){
				char* input1 = malloc(50 * sizeof(char));
				char* output1 = malloc(50 * sizeof(char));
				fscanf(fp, "%s", input1);
				fscanf(fp, "%s", output1);
				int inputCount = 0;
				struct variable* functionInput = malloc(sizeof(struct variable) * 1);
				for(int i = 0; i < numOfInputs; i++){
					if(strcmp(input1, "1") == 0 && functionInput[0].valid != 1){
						functionInput[0].value = 1;
						functionInput[0].valid =1;
						inputCount++;
					} else if(strcmp(input1, "0") == 0&&functionInput[0].valid != 1){
						functionInput[0].value = 0;
						functionInput[0].valid =1;
						inputCount++;
					}

					if(strcmp(inputArray[i].name, input1) == 0 && functionInput[0].valid != 1){
						functionInput[0] = inputArray[i];
						functionInput[0].valid = 1;
						inputCount++;
					}
				}
				//search temporary array for inputs -- add a flag to know if there s a value
				for(int i = 0; i < 50; i++){
					if(tempArray[i].valid != 1){
						break;
					}
					else{
						if(strcmp(tempArray[i].name, input1) == 0){
							functionInput[0] = tempArray[i];
							inputCount++;
						}
					}
				}
				if(inputCount != 1){
					continue;
				}
				struct variable output;
				output.value = notGate(functionInput[0].value);
				free(functionInput);
				output.name = output1;
				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "DECODER") == 0){
				int decoderInputs = 0;
				fscanf(fp, "%d", &decoderInputs);
				int decoderOutputs;
				decoderOutputs = pow(2,decoderInputs);
				struct variable* decoderInputArray = malloc(sizeof(struct variable)*decoderInputs);
				struct variable* decoderOutputArray = malloc(sizeof(struct variable)*decoderOutputs);
				char* temporary = malloc(50 * sizeof(char));
				//getting input names
				int inputCount = 0;
				for(int i = 0; i< decoderInputs; i++){
					fscanf(fp, "%s", temporary);
					if(strcmp(temporary, "0") == 0){
						decoderInputArray[i].value = 0;
						decoderInputArray[i].valid = 1;
						inputCount++;
					}else if(strcmp(temporary, "1") == 0){
						decoderInputArray[i].value = 1;
						decoderInputArray[i].valid = 1;
						inputCount++;
					}else{
					char* new = malloc(50 * sizeof(char));
					decoderInputArray[i].name = strcpy(new,temporary);
					}
				}
				//getting output names
				for(int i = 0; i< decoderOutputs; i++){
					fscanf(fp, "%s", temporary);
					char* new = malloc(50 * sizeof(char));
					decoderOutputArray[i].name = strcpy(new,temporary);
				}
				//checking decoder inputs in input vars
				for(int i = 0; i < decoderInputs; i++){
					for(int j = 0; j < numOfInputs;j++){
						if(decoderInputArray[i].valid == 1){
							continue;
						}
						if(strcmp(decoderInputArray[i].name, inputArray[j].name)==0){
							decoderInputArray[i].value = inputArray[j].value;
							decoderInputArray[i].valid = 1;
							inputCount++;
						}
					}
				}
				//checking decoder inputs in temp
				for(int i = 0; i < decoderInputs; i++){
					for(int j = 0; j < 50; j++){
						if(decoderInputArray[i].valid == 1){
							continue;
						}
						if(tempArray[i].valid != 1){
							break;
						}
						if(strcmp(decoderInputArray[i].name, tempArray[j].name) == 0){
							decoderInputArray[i].value = tempArray[j].value;
							decoderInputArray[i].valid = 1;
							inputCount++;
						}
					}
				}
				if(inputCount != decoderInputs){
					continue;
				}
				struct variable* decoderFinalOutput = decoder(decoderInputs, decoderOutputs, decoderInputArray, decoderOutputArray);

				//check if each output is a output of ckt
				for(int i = 0; i < decoderOutputs; i++){
					for(int j = 0; j < numOfOutputs; j++){
						if(strcmp(decoderFinalOutput[i].name, outputArray[j].name) == 0){
							outputArray[j].value = decoderOutputArray[i].value;
							decoderFinalOutput[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}

				//add to temp
				for(int i = 0; i < decoderOutputs; i++){
					if(decoderFinalOutput[i].valid != 1){
						for(int j = 0;j < 50; j++){
							if(tempArray[j].valid != 1){
								tempArray[j].name = decoderFinalOutput[i].name;
								tempArray[j].value = decoderFinalOutput[i].value;
								tempArray[j].valid = 1;
								boolArray[numOfLinesCounter] = 1;
								break;
							}
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
			else if(strcmp(word, "MULTIPLEXER") == 0){
				//calculating # of inputs and selectors
				int muxInputs = 0;
				fscanf(fp, "%d", &muxInputs);

				int selectors = 0;
				selectors = log2(muxInputs);
				struct variable* muxArray = malloc(sizeof(struct variable)*muxInputs);
				struct variable* selectorArray = malloc(sizeof(struct variable)* selectors);
				int inputCount = 0;
				int selectorCount = 0;
				char* temporary = malloc(50 * sizeof(char));
				//getting input names
				for(int i = 0; i< muxInputs; i++){
					fscanf(fp, "%s", temporary);
					if(strcmp(temporary, "0") == 0){
						muxArray[i].value = 0;
						muxArray[i].valid = 1;
						inputCount++;
					}else if(strcmp(temporary, "1") == 0){
						muxArray[i].value = 1;
						muxArray[i].valid = 1;
						inputCount++;
					}else{
					char* new = malloc(50 * sizeof(char));
					muxArray[i].name = strcpy(new,temporary);
					}
				}
				//getting selector names
				for(int i = 0; i < selectors; i++){
					fscanf(fp, "%s", temporary);
					if(strcmp(temporary, "0") == 0){
						selectorArray[i].value = 0;
						selectorArray[i].valid = 1;
						selectorCount++;
					}else if(strcmp(temporary, "1") == 0){
						selectorArray[i].value = 1;
						selectorArray[i].valid = 1;
						selectorCount++;
					}else{
					char* new = malloc(50 * sizeof(char));
					selectorArray[i].name = strcpy(new,temporary);
					}
				}
				//checking mux inputs in input vars
				for(int i = 0; i < muxInputs; i++){
					for(int j = 0; j < numOfInputs;j++){
						if(muxArray[i].valid == 1){
							continue;
						}
						if(strcmp(muxArray[i].name, inputArray[j].name)==0){
							muxArray[i].value = inputArray[j].value;
							muxArray[i].valid = 1;
							inputCount++;
						}
					}
				}
				//checking mux inputs in temp
				for(int i = 0; i < muxInputs; i++){
					for(int j = 0; j < 50; j++){
						if(muxArray[i].valid == 1){
							continue;
						}
						if(tempArray[i].valid != 1){
							break;
						}
						if(strcmp(muxArray[i].name, tempArray[j].name) == 0){
							muxArray[i].value = tempArray[j].value;
							muxArray[i].valid = 1;
							inputCount++;
						}
					}
				}
				//checking selectors in input vars
				for(int i = 0; i < selectors; i++){
					for(int j = 0; j < numOfInputs;j++){
						if(selectorArray[i].valid == 1){
							continue;
						}
						if(strcmp(selectorArray[i].name, inputArray[j].name)==0){
							selectorArray[i].value = inputArray[j].value;
							selectorArray[i].valid = 1;
							selectorCount++;
						}
					}
				}
				//checking selectors in temp
				for(int i = 0; i < selectors; i++){
					for(int j = 0; j < 50; j++){
						if(selectorArray[i].valid == 1){
							continue;
						}
						if(tempArray[i].valid != 1){
							break;
						}
						if(strcmp(selectorArray[i].name, tempArray[j].name) == 0){
							selectorArray[i].value = tempArray[j].value;
							selectorArray[i].valid = 1;
							selectorCount++;
						}
					}
				}
				if(inputCount != muxInputs ||selectorCount != selectors){
					continue;
				}
				struct variable output;
				output.value = multiplexer(muxInputs, selectors, muxArray, selectorArray);
				char* output1 = malloc(sizeof(char)*50);
				fscanf(fp, "%s", output1);
				output.name = output1;

				//check if output of circuit -- if not add to temp array
				int flag = 0;
				for(int i = 0; i < numOfOutputs; i++){
					if(strcmp(outputArray[i].name, output.name) == 0){
						outputArray[i].value = output.value;
						flag = 1;
						boolArray[numOfLinesCounter] = 1;
					}
				}
				if(flag == 0){
					for(int i = 0;i < 50; i++){
						if(tempArray[i].valid != 1){
							tempArray[i].name = output.name;
							tempArray[i].value = output.value;
							tempArray[i].valid = 1;
							boolArray[numOfLinesCounter] = 1;
							break;
						}
					}
				}
				boolArray[numOfLinesCounter] = 1;
			}
		}
		if(checkBoolArray(boolArray, numOfLines)){
			recurseflag = 1;
			boolArray = initializeBoolArray(boolArray, numOfLines);
			rewind(fp);
		}
		else{
			rewind(fp);
		}
	}
		for(int i = 0; i < numOfOutputs; i++){
			printf("%d	", outputArray[i].value);
			//fprintf(fp2,"%d	", outputArray[i].value);
		}
		free(tempArray);
		printf("\n");
		//fprintf(fp2, "\n");
		rewind(fp);
	}




	/* create linked list for temp variables -- have to search through and compare names if used (clear after each gray code)
	need to create functions for OR AND etc.
	need to write function to calculate gray code and put into matrix
	rewind the fscanf for each gray code
	*/

	//have to figure out how to decide which inputs go into which gate


	return 0;
}


int calculateGrayCode(int binary){
	int grayCode = 0;
	grayCode = (binary >> 1) ^ binary;
	return grayCode;
}

int andGate(int one, int two){
	return (one & two);
}

int orGate(int one, int two){
	return (one | two);
}

int notGate(int one){
	if(one ==1){
		return 0;
	}
	else{
		return 1;
	}
}

int xorGate(int one, int two){
	return (one ^ two);
}

int nandGate(int one, int two){
	int temp = andGate(one, two);
	return notGate(temp);
}

int norGate(int one, int two){
	int temp = orGate(one,two);
	if(temp ==1){
		return 0;
	}
	else{
		return 1;
	}
}

int xnorGate(int one, int two){
	int temp = xorGate(one, two);
	if(temp ==1){
		return 0;
	}
	else{
		return 1;
	}
}

int multiplexer(int muxInputs, int selectors, struct variable* muxArray, struct variable* selectorArray){
	for(int i = 0; i < muxInputs; i++){
		muxArray[i].muxGrayCode = calculateGrayCode(i);
	}
	int decimal = calculateBitsToDec(selectors, selectorArray);
	for(int i = 0; i < muxInputs; i++){
		if(muxArray[i].muxGrayCode == decimal){
			return muxArray[i].value;
		}
	}
	return 0;
}

int calculateBitsToDec(int numOfBits, struct variable* selectorArray){
	int decimal = 0;
	for(int i = 0; i < numOfBits; i++){
		decimal = decimal + pow(2,numOfBits-i-1)*selectorArray[i].value;
	}
	//decimal++;
	return decimal;
}

struct variable* decoder(int decoderInputs, int decoderOutputs, struct variable* decoderInputArray, struct variable* decoderOutputArray){
	for(int i = 0; i < decoderOutputs; i++){
		decoderOutputArray[i].muxGrayCode = calculateGrayCode(i);
	}
	int decimal = calculateBitsToDec(decoderInputs, decoderInputArray);
	for(int i = 0; i < decoderOutputs; i++){
		if(decoderOutputArray[i].muxGrayCode == decimal){
			decoderOutputArray[i].value = 1;
		}
		else{
			decoderOutputArray[i].value = 0;
		}
	}
	return decoderOutputArray;
}

int* initializeBoolArray(int*boolArray, int numOfLines){
	for(int i = 0; i< numOfLines; i++){
		boolArray[i] = 0;
	}
	return boolArray;
}

bool checkBoolArray(int* boolArray, int numOfLines){
	for(int i = 0; i < numOfLines; i++){
		if(boolArray[i] == 0){
			return false;
		}
	}
	return true;
}
//DECODER
//MULTIPLEXER
// input arrays as idk exactly how many inputs each time
//have to account for just 1s and 0s too



/*int calculatePowerOfTwo(int number){
	double count = 0;
	count = pow(2,number);
	int returnValue = count;
	return returnValue;
}*/
