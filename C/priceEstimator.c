#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

double divisor = 1;
double multiplier = 1;
double ** inverseMatrix(double ** matrix, int rows, int columns);
double** matrixMultiplication(double** finalMatrix, double** matrix1, double** matrix2, int rows, int columns, int num2);
double multiplyFunction(int row1, int column1, int row2, int column2, int columns, double**matrix1, double **matrix2);
double ** transposeMatrix(double** matrix, int rows, int columns);
void estimatePrice(double ** matrixW, double** testMatrix, int numOfHouses, int numOfAttributes);
void freeMatrix(double** matrix, int counter);

int main(int argc, char** argv){
	if(argc != 3){
		printf("insufficient arguments\n");
		exit(0);
	}
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("file does not exist\n");
		exit(0);
	}
	int numOfExamples = 0;
	int numOfAttributes = 0;

	if(fscanf(fp, "%d", &numOfAttributes) == EOF){
		printf("empty file\n");
		exit(0);
	}
	fscanf(fp, "%d", &numOfExamples);
	
	//allocate matrix
	double **matrixX = malloc(sizeof(double*)*numOfExamples);
	for(int i = 0; i < numOfExamples; i++){
		matrixX[i] = malloc(sizeof(double)*(numOfAttributes+1));
	}

	double **matrixY = malloc(sizeof(double*)*numOfExamples);
	for (int i = 0; i < numOfExamples; i++){
		matrixY[i] = malloc(sizeof(double));
	}

	//populate matrix
	double num = 0;
	
	for(int i = 0; i < numOfExamples; i++){
		for(int j = 0; j < numOfAttributes+2; j++){
			//adds in 1s at beginning
			if(j == 0){
				matrixX[i][j] = 1.00000;
				
			}
			else if(j != (numOfAttributes+1)){
				fscanf(fp, "%lf, ",&num);
				
				matrixX[i][j] = num;
				
			}
			else{
				fscanf(fp, "%lf, ", &num);
				matrixY[i][0] = num;
				
			}
		}
	}
	
	//calculates equation W = (Xt.X)-1.Xt.Y

	//Xt
	double **transposedMatrix = transposeMatrix(matrixX, numOfExamples, numOfAttributes);
	
	double **final = malloc(sizeof(double*) *numOfExamples);
	for(int i = 0; i < numOfExamples; i++){
		final[i] = malloc(sizeof(double)*numOfExamples);
	}
	
	//Xt.X
	double **multipliedMatrix = matrixMultiplication(final, transposedMatrix, matrixX,numOfAttributes+1,numOfAttributes+1,numOfExamples);
	//(Xt.X)-1
	double ** inversedMatrix = inverseMatrix(multipliedMatrix, numOfAttributes+1, numOfAttributes+1);
	
	double** finalMatrix = malloc(sizeof(double*) *numOfExamples);
	for(int i = 0; i < numOfExamples; i++){
		finalMatrix[i] = malloc(sizeof(double)*(numOfExamples));
	}
	//(Xt.X)-1.Xt
	double ** multipliedMatrix2 = matrixMultiplication(finalMatrix,inversedMatrix,transposedMatrix,(numOfAttributes+1),numOfExamples,(numOfAttributes+1));
	
	double** matrixW = malloc(sizeof(double*) *numOfExamples);
	for(int i = 0; i < numOfExamples; i++){
		matrixW[i] = malloc(sizeof(double)*numOfExamples);
	}
	//multiplied matrix 3 is the final output from the equation
	//(Xt.X)-1.Xt.Y
	double ** multipliedMatrix3 = matrixMultiplication(matrixW, multipliedMatrix2, matrixY,numOfAttributes+1,1,numOfExamples);
	

	//TEST data
	FILE *fp2 = fopen(argv[2], "r");
	if(fp == NULL){
		printf("file does not exist\n");
		exit(0);
	}
	int numOfHouses = 0;
	fscanf(fp2, "%d", &numOfHouses);
	double** testMatrix = malloc(sizeof(double*)* (numOfHouses+1));
	for(int i = 0; i < numOfHouses; i++){
		testMatrix[i] = malloc(sizeof(double)*(numOfAttributes+1));
	}
	for (int i = 0; i < numOfHouses; i++){
	
		for(int j = 0; j < numOfAttributes;j++){
		
			double num = 0;
			fscanf(fp2, "%lf, ", &num);
			testMatrix[i][j] = num;
	
		}
	}

	//estimates price based on training data final matrix and test data
	estimatePrice(multipliedMatrix3,testMatrix, numOfHouses, numOfAttributes);
	
	//frees all matrices
	freeMatrix(matrixX, numOfExamples);
	freeMatrix(matrixY, numOfExamples);
	freeMatrix(transposedMatrix, (numOfAttributes+1));
	freeMatrix(inversedMatrix, numOfAttributes);
	freeMatrix(multipliedMatrix, numOfExamples);
	freeMatrix(multipliedMatrix2, numOfExamples);
	freeMatrix(multipliedMatrix3, numOfExamples);
	return 0;
}

	//multiplies matrices
double** matrixMultiplication(double** finalMatrix, double** matrix1, double** matrix2, int rows, int columns, int num2){
	//populate matrix
	int matrix1Row = 0;
	//loops rows
	for (int i = 0; i < rows; i++){
		int matrix2Column = 0;
		//loops columns
		for(int j = 0; j < columns; j++){
			int matrix2Row = 0;
			int matrix1Column = 0;
			finalMatrix[i][j] = multiplyFunction(matrix1Row, matrix1Column,matrix2Row, matrix2Column, num2, matrix1, matrix2);
	matrix2Column++;

		}
		matrix1Row++;

	}
	return finalMatrix;
}

//summation of the multiplication of elements in column and row of respective matrix
double multiplyFunction(int row1, int column1, int row2, int column2, int columns, double**matrix1, double **matrix2){
	double returnValue = 0;
	for(int i = 0; i < columns; i++){
		returnValue = returnValue + (matrix1[row1][column1]*matrix2[row2][column2]);
		row2++;
		column1++;
	}
	return returnValue;
}


double ** inverseMatrix(double ** matrix, int rows, int columns){
	double ** concatMatrix = malloc(sizeof(double*)*rows);
	for(int i = 0; i < rows; i++){
		concatMatrix[i] = malloc(sizeof(double)*columns*2);
	}
	//adds identity matrix
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			concatMatrix[i][j] = matrix[i][j];
			
		}
		for(int j = columns; j < (columns*2); j++){
			if((j-columns) == i){
			concatMatrix[i][j] = 1;
			}
			else{
			concatMatrix[i][j] = 0;
			}
	
		}
	}

	
	//divides when i == j and subtracts when i <j
	for(int j = 0; j < columns; j++){
		for(int i = 0; i < rows; i++){
			if(i == j && concatMatrix[i][j] != 1){
				//loop to divide whole row by constant
				double constVal = concatMatrix[i][j];
				for(int k = j; k < (columns*2); k++){
					concatMatrix[i][k] = concatMatrix[i][k]/constVal;
				}
			}
			if(i > j && concatMatrix[i][j] != 0){
				//loops to subtact rowi by constant*rowj 
				double constVal = concatMatrix[i][j];
				for(int k = j; k<(columns*2);k++){
					concatMatrix[i][k] = concatMatrix[i][k] - (concatMatrix[j][k]*constVal);
				}
			}
		}
	}
	
	for(int j = 0; j < columns; j++){
		for(int i = 0; i < rows; i++){
			if(i < j && concatMatrix[i][j] != 0){
				double constVal = concatMatrix[i][j];
				for(int k = j; k < (columns*2); k++){
					
					concatMatrix[i][k] = concatMatrix[i][k] - (concatMatrix[j][k]*constVal);
				
				}
			}
		}
	}
		
	double ** inverseMatrix = malloc(sizeof(double*) * rows);
	for(int i = 0; i < columns; i++){
		inverseMatrix[i] = malloc(sizeof(double)*columns);
	}
	
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
		
			inverseMatrix[i][j] = concatMatrix[i][j+columns];
			
		}
	}
	freeMatrix(concatMatrix, (rows));
	return inverseMatrix;
}


double ** transposeMatrix(double** matrix, int rows, int columns){
	double **transposedMatrix = malloc(sizeof(double*) *(columns+1));
	for(int i = 0; i < columns+1; i++){
		transposedMatrix[i] = malloc(sizeof(double)*rows);
	}
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < (columns+1);j++){
			transposedMatrix[j][i] = matrix[i][j];
		}
		}
	return transposedMatrix;
}

void estimatePrice(double ** matrixW, double** testMatrix, int numOfHouses, int numOfAttributes){
	double* finalAnswer = malloc(sizeof(double)*numOfHouses);
	for(int i = 0; i < numOfHouses; i++){
		finalAnswer[i] = matrixW[0][0];
		for(int j = 1; j < (numOfAttributes+1); j++){
			finalAnswer[i] = finalAnswer[i] + matrixW[j][0]*testMatrix[i][j-1];
		}
		printf("%0.0lf\n",finalAnswer[i]);
	}
	free(finalAnswer);
	
			
}

void freeMatrix(double** matrix, int counter){
	for(int i = 0; i < counter; i++){
		free(matrix[i]);
	}
	free(matrix);
}
