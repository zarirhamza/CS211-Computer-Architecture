#include <stdlib.h>
#include <stdio.h>

double** createMatrixY(FILE* fp, int row, int column){
	double** matrix = (double**)malloc(sizeof(double**)*row);
	double ignore;
	for(int i = 0; i < row; i++){
		matrix[i] = (double*)malloc(sizeof(double*));
		fscanf(fp,"%lf,",&matrix[i][0]);
		for(int j = 1; j < column; j++){
			fscanf(fp,"%lf,",&ignore);
		}
	}
	return matrix;
}


double** createMatrixX(FILE* fp, int row, int column){
	double** matrix = (double**)malloc(sizeof(double**)*row);
	double ignore;
	for(int i = 0; i < row; i++){
		matrix[i] = (double* )malloc(sizeof(double*)*column);
		fscanf(fp,"%lf,",&ignore);
		matrix[i][0] = 1;
		for(int j = 1; j < column; j++){
			fscanf(fp,"%lf,",&matrix[i][j]);
		}
	}
	return matrix;
}


double** createMatrixXTest(FILE* fp, int row, int column){
	double** matrix = (double**)malloc(sizeof(double**)*row);
	for(int i = 0; i < row; i++){
		matrix[i] = (double* )malloc(sizeof(double*)*column);
		matrix[i][0] = 1;
		for(int j = 1; j < column; j++){
			fscanf(fp,"%lf,",&matrix[i][j]);
		}
	}
	return matrix;
}

double** transposeMatrix(double** matrix, int row, int column){
	double** matrixt = (double**)malloc(sizeof(double**)*column);
	for(int i = 0; i < column; i++){
		matrixt[i] = (double*)malloc(sizeof(double*)*row);
		for(int j = 0; j < row; j++){
			matrixt[i][j] = matrix[j][i];
		}
	}
	return matrixt;
}


double** multiplyMatrix(double** matrix1, int row1, int column1, double** matrix2, int row2, int column2){
	double** matrixM = (double**)malloc(sizeof(double**)*row1);
	for(int i = 0; i < row1; i++){
		matrixM[i] = (double*)malloc(sizeof(double*)*column2);
		for(int j = 0; j < column2; j++){
			for(int z = 0; z < column1; z++){
				matrixM[i][j] += matrix1[i][z] * matrix2[z][j];
			}
			//printf("%lf ", matrixM[i][j]);
		}
		//printf("\n");
	}
	return matrixM;
}

void printMatrix(double** matrix, int row, int column){
	for (int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			printf("%0.0lf\n", matrix[i][j]);
		}
	}

}


double** inverseMatrix(double** matrix, int row, int column){
	double** matrixI = (double**)malloc(sizeof(double**)*row);
	for(int i = 0; i < column; i++){
		matrixI[i] = (double*)malloc(sizeof(double*)*column);
	}


	for(int i = 0; i < column; i++){
		for(int j = 0; j < row; j++){
			if(i == j)
				matrixI[i][j] = 1;
			else
				matrixI[i][j] = 0;
		}
	}
	
	
	for(int j = 0; j < column; j++){
		for(int i = j; i < row; i++){
			char symbol[1];
			double val;
			if(i == j){
				symbol[0] = '/';
				val = matrix[i][j];
			}
			else if(i != j){
				//printf("%d,%d\n",i,j);
				symbol[0] = '*';	
				val = matrix[i][j] / matrix[j][j];
			}
			for(int k = 0; k < column; k++){
				//printf("nume is %d, %d, %d, %c\n", i,j,k,symbol[0]);
				if(symbol[0] == '/'){
					matrix[i][k] = matrix[i][k] / val;
					matrixI[i][k] = matrixI[i][k] / val;
				}
				else if(symbol[0] == '*'){
					//printf("nume is %d, %d, %d\n", i,j,k);
					matrix[i][k] = matrix[i][k] - ( matrix[j][k] * val);
					matrixI[i][k] = matrixI[i][k] - (matrixI[j][k] * val);
				}
				//printMatrix(matrix, row, column);
				//printf("\n");
				//printMatrix(matrixI, row, column);
				//printf("\n");
			}
		}
	}

	for(int j = 1; j < column; j++){
		for(int i = 0; i < j; i++){//row
			double val = matrix[i][j]/matrix[j][j];
			for(int k = 0; k < column; k++){
				matrix[i][k] = matrix[i][k] - (matrix[j][k]*val);
				matrixI[i][k] = matrixI[i][k] - (matrixI[j][k] * val);
			}
		}
	}
	return matrixI;
}


void freeMatrix(double** matrix, int rows){
	for(int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

 
int main (int argc, char** argv) {
	if(argc != 3){
		printf("Error %d\n", argc);
		return 0;
	}

	FILE* trainer = fopen(argv[1],"r");
	int columns, rows;

	//create matrices with row = rows and column = columns

	fscanf(trainer,"%d",&columns);
	fscanf(trainer,"%d",&rows);
	columns = columns + 1;
	double** matrixX = createMatrixX(trainer, rows, columns);
	
	rewind(trainer);
	fscanf(trainer,"%d",&columns);
	columns = columns + 1;
	fscanf(trainer,"%d",&rows);	
	double** matrixY = createMatrixY(trainer, rows, columns);

	//printMatrix(matrixX, rows, columns);
	//printf("\n");
	//printMatrix(matrixY, rows, 1);
	//printf("\n");

	//create transpose - row = columns, and columns = rows
	
	double** transposeX = transposeMatrix(matrixX, rows, columns);
	//printMatrix(transposeX, columns, rows);
	//printf("\n");
	
	//multiply Matrices = row = row and column = row	
	double** xTranposeX = multiplyMatrix(transposeX, columns, rows, matrixX, rows, columns);
	//printMatrix(xTranposeX, columns, columns);
	//printf("\n");
	//inverse
	double** inverseXT = inverseMatrix(xTranposeX, columns, columns);
	//printMatrix(inverseXT, columns, columns);
	//printf("\n");
	//multiply matrices

	double** xTransposeXInverseXT = multiplyMatrix(inverseXT, columns, columns, transposeX, columns, rows);
	//printMatrix(xTransposeXInverseXT, columns, rows);
	//printf("\n");
	//multiple again
	double** matrixW = multiplyMatrix(xTransposeXInverseXT, columns, rows, matrixY, rows, 1);
	//printMatrix(matrixW, columns, 1);
	//printf("\n");
	//store value
	

	FILE* test = fopen(argv[2],"r");
	int rowTests;
	fscanf(test,"%d",&rowTests);

	//create matrix with given row number and same columns
	double** matrixXTest = createMatrixXTest(test, rowTests, columns);
	//printMatrix(matrixXTest, rowTests, columns);
	//printf("\n");
	//multiple this with stored value
	double** answer = multiplyMatrix(matrixXTest, rowTests, columns, matrixW, columns, 1);
	//print value of each row
	printMatrix(answer, rowTests, 1);	

	//free matrixes

	freeMatrix(matrixX, rows);
	freeMatrix(matrixY, rows);
	freeMatrix(transposeX, columns) ;
	freeMatrix(xTranposeX, columns);
	freeMatrix(inverseXT, columns);
	freeMatrix(xTransposeXInverseXT, columns);
	freeMatrix(matrixW, columns);
	freeMatrix(matrixXTest, rowTests);
	freeMatrix(answer, rowTests);

	fclose(trainer);
	fclose(test);
	return 0;	
}
