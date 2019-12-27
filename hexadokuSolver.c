#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int convertToInteger(char* character);
int* getRow(int** matrix, int rowNumber);
int* getColumn(int** matrix, int columnNumber);
int** getQuadrant(int** matrix, int currentRow, int currentColumn);
int getCounter(int current);
bool* checkArray(int* current);
bool* checkQuadrant(int** current);
bool* initializeToTrue(bool* boolArray);
bool checkInitialMatrix(int** matrix);
bool checkAllQuadrants(int ** matrix, bool* boolArray, int lowerXBound, int upperXBound, int lowerYBound, int upperYBound);


int main(int argc, char** argv){
	if (argc < 2){
		printf("insufficient arguments\n");
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		printf("file does not exist\n");
		exit(0);
	}
	char* character = malloc(sizeof(char)*16);
	int ** matrix;
	matrix = malloc(sizeof(int*)*16);
	
	//allocate
	for(int i = 0; i < 16; i++){
			matrix[i] = malloc(16*sizeof(int));
	}

	//populate
	
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			//scan as character and convert to int 
			fscanf(fp, "%s", character);
			matrix[i][j] = convertToInteger(character);
		}
	}
	//checks initial matrix validity
	if(!checkInitialMatrix(matrix)){
		printf("no-solution\n");
		exit(0);
	}
	
	//loop through matrix
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			//if it is a '-'
			if(matrix[i][j] == -1){
				int* counterArray = malloc(sizeof(int)*16);
				for(int k = 0; k < 16; k++){
					counterArray[k] = 0;
				}

				int* currentRow = getRow(matrix, i);
				int* currentColumn = getColumn(matrix, j);
				int** currentQuad = getQuadrant(matrix, i, j);

				bool* rowBool = checkArray(currentRow);
				bool* columnBool = checkArray(currentColumn);
				bool* quadBool = checkQuadrant(currentQuad);
				for(int n = 0; n < 16; n++){
					int counter = 0;
					if(rowBool[n] == true){
						counter++;
					}
					if(columnBool[n] == true){
						counter++;
					}
					if(quadBool[n] == true){
						counter++;
					}
					counterArray[n] = counter;

				}
				
				int check = 0;
				for (int m = 0x0; m < 0x10; m++){
					if(counterArray[m] == 0x03){
						matrix[i][j] = m;
						check++;
					}
				}
				if(check < 1){
					printf("no-solution 123");
					exit(0);
				}
				
				//frees all malloc'd memory
				free(counterArray);
				free(currentRow);
				free(currentColumn);
				for(int i = 0; i < 4;i++){
					free(currentQuad[i]);
				}
				free(currentQuad);
				free(rowBool);
				free(columnBool);
				free(quadBool);
			}
		}
	}
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			printf("%X	", matrix[i][j]);
		}
		printf("\n");
	}	

	for(int i = 0; i< 16; i++){
		free(matrix[i]);
	}
	free(matrix);
	 
	return 0;
}



//returns current row
int* getRow(int** matrix, int rowNumber){
	int * currentRow = malloc(sizeof(int)*16);
	for(int i = 0; i < 16; i++){
		currentRow[i] = matrix[rowNumber][i];
	}
	return currentRow;
}

//returns current column
int* getColumn(int** matrix, int columnNumber){
	int * currentColumn = malloc(sizeof(int)*16);
	for(int i = 0; i < 16; i++){
		currentColumn[i] = matrix[i][columnNumber];
	}
	return currentColumn;
}

//gets the current quadrant
int** getQuadrant(int** matrix, int currentRow, int currentColumn){
	int ** quadrant = malloc(sizeof(int*)*4);
	for(int i = 0; i < 4; i++){
		quadrant[i] = malloc(sizeof(int)*4);
	}
	int rowCounter = getCounter(currentRow);
	int columnCounter = getCounter(currentColumn);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			quadrant[i][j] = matrix[i+rowCounter][j+columnCounter];
		}
	}
	return quadrant;
}

//converts input char to int(hex)
int convertToInteger(char* character){
	int num;
	//printf("character: %c", character);
	switch(character[0]){
		case('0'):num = 0x0;
			break;
		case('1'):num = 0x1;
			break;
		case('2'):num = 0x2;
			break;
		case('3'):num = 0x3;
			break;
		case('4'):num = 0x4;
			break;
		case('5'):num = 0x5;
			break;
		case('6'):num = 0x6;
			break;
		case('7'):num = 0x7;
			break;
		case('8'):num = 0x8;
			break;
		case('9'):num = 0x9;
			break;
		case('A'):num = 0xA;
			break;
		case('B'):num = 0xB;
			break;
		case('C'):num = 0xC;
			break;
		case('D'):num = 0xD;
			break;
		case('E'):num = 0xE;
			break;
		case('F'):num = 0xF;
			break;
		default: num = -1;

	}
	return num;
}
	
//counter yo determine starting points for quadrant
int getCounter(int current){
	int counter;
	if(current <= 3){
		counter = 0;
	}
	else if(current <= 7){
		counter = 4;
	}
	else if(current <= 11){
		counter = 8;
	}
	else{
		counter = 12;
	}
	return counter;
}	

//returns available numbers for current array(row/column)
bool* checkArray(int* current){
	bool* availableNumbers = malloc(sizeof(bool)*16);
	for(int i = 0; i < 16; i++){
		availableNumbers[i] = true;
	}
	for(int i = 0; i < 16; i++){
		if(current[i] != -1){
			availableNumbers[current[i]] = false;
		}
	}
	return availableNumbers;
	
}

//returns available numbers for current quadrant
bool* checkQuadrant(int** current){
	bool* availableNumbers = malloc(sizeof(bool)*16);
	for(int i = 0; i < 16; i++){
		availableNumbers[i] = true;
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
		if(current[i][j] != -1){
			availableNumbers[current[i][j]] = false;
		}
	}
	}
	return availableNumbers;
}


//checks whether each row, column and quadrant are valid for input
bool checkInitialMatrix(int** matrix){
	bool* boolArrayOne = malloc(sizeof(bool)*16);
	boolArrayOne = initializeToTrue(boolArrayOne);
	//rows
	for(int i = 0; i < 16; i++){
		boolArrayOne = initializeToTrue(boolArrayOne);
		for(int j = 0; j <16; j++){
			if(matrix[i][j] == -1 || matrix[i][j] == 0xFFFFFFFF){
				continue;
			}
			if(boolArrayOne[matrix[i][j]] == false){
				return false;
			}
			boolArrayOne[matrix[i][j]] = false;
		}
	}
	//columns
	boolArrayOne = initializeToTrue(boolArrayOne);
	for(int i = 0; i < 16; i++){
		boolArrayOne = initializeToTrue(boolArrayOne);
		for(int j = 0; j <16; j++){
			if(matrix[j][i] == -1){
				continue;
			}
			if(boolArrayOne[matrix[j][i]] == false){
				return false;
			}
			boolArrayOne[matrix[j][i]] = false;
		}
	}
	//quadrants
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 0, 4, 0, 4)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 0, 4, 4, 8)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 0, 4, 8, 12)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 0, 4, 12, 16)){
		return false;
	}

	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 4, 8, 0, 4)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 4, 8, 4, 8)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 4, 8, 8, 12)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 4, 8, 12, 16)){
		return false;
	}

	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 8, 12, 0, 4)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 8, 12, 4, 8)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 8, 12, 8, 12)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 8, 12, 12, 16)){
		return false;
	}
	
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 12, 16, 0, 4)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 12, 16, 4, 8)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 12, 16, 8, 12)){
		return false;
	}
	boolArrayOne = initializeToTrue(boolArrayOne);
	if(!checkAllQuadrants(matrix, boolArrayOne, 12, 16, 12, 16)){
		return false;
	}
	
	
	return true;
}

//initializes bool array to true
bool* initializeToTrue(bool* boolArray){
	for(int i = 0; i < 16; i++){
		boolArray[i] = true;
	}
	return boolArray;
}


//checks all initial quadrants to make sure input is valid
bool checkAllQuadrants(int ** matrix, bool* boolArrayOne, int lowerXBound, int upperXBound, int lowerYBound, int upperYBound){
	for(int i = lowerXBound; i < upperXBound; i++){
		for(int j = lowerYBound; j < upperYBound; j++){
			if(matrix[i][j] == -1){
				continue;
			}
			if(boolArrayOne[matrix[i][j]] == false){
				return false;
			}
			boolArrayOne[matrix[i][j]] = false;
			
		}
	}
	return true;
}
