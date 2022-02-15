#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

double* generateIdentity(int dim, double* temp);
void inverse(double* matrix, int dim, double* temp);
void transpose(double* matrix, int r, int c, double* temp);
void multiply(double* matrix1, double* matrix2, int r1, int c1, int r2, int c2, double* temp);

double* generateIdentity(int dim, double* temp){
  int x;
  int y;

  for (x = 0; x < dim; x++){
    for (y = 0; y < dim; y++){
      if (x == y){
        temp[x*dim + y] = 1;
      }
      else{
        temp[x*dim + y] = 0;
      }
    }
  }

  return temp;
}

void inverse(double* matrix, int dim, double* temp){
	temp = generateIdentity(dim, temp);
	int i, j, p, k, l;
	double f;		
	
	for (p = 0; p < dim; p++){
		f = matrix[p*dim + p];
		for (j = 0; j < dim; j++){		//divides each element of the pivot row by the pivot element
			matrix[p*dim + j] /= f;
			temp[p*dim + j] /= f;
		}
		
		for (i = p+1; i < dim; i++){	//subtracts the pivot row from each other one to create 0s below the pivot
			f = matrix[i*dim + p];	
			for (k = 0; k < dim; k++){
				matrix[i*dim + k] -= (matrix[p*dim + k] * f);
				temp[i*dim + k] -= (temp[p*dim + k] * f);
			}
		
		}
	}
	
	//left with an upper triangular matrix
		
		for (p = dim-1; p >= 0; p--){
			for (i = p-1; i >= 0; i--){
				f = matrix[i*dim + p];
				for (l = dim-1; l >=0; l--){
					matrix[i*dim + l] -= (matrix[p*dim + l] * f);
					temp[i*dim + l] -= (temp[p*dim + l] * f);
				}
			}
		}
}

void transpose(double* matrix, int r, int c, double* temp){
	
	int i, j;
	
	for (i = 0; i < r; i++){
		for (j = 0; j < c; j++){
			temp[j*r + i] = matrix[i*c + j];
		}
	}
	
}

void multiply(double* matrix1, double* matrix2, int r1, int c1, int r2, int c2, double* temp){

  int i;
  int j;
  int k;

  for (i = 0; i < r1; i++){
    for (j = 0; j < c2; j++){
      double sum = 0;
      for (k = 0; k < c1; k++){
        sum += matrix1[i*c1 + k] * matrix2[k*c2 + j];
      }
	  temp[i*c2 + j] = sum;
    }
  }
}



int main(int argc, char *argv[]){

	if (argc < 2 || argv[1][0] == '\0' || argv[2][0] == '\0'){
		printf("Error, Usage: ./estimate train_file.txt test_file.txt");
		return 1;
	}
	
	int r_train, c_train;
	FILE *fp;
	char train[10];
	double* X;
	double* X_prices;

  	fp = fopen(argv[1], "r");
  	fscanf(fp, "%5s", train);
  	fscanf(fp, "%d", &c_train);
  	fscanf(fp, "%d", &r_train);

	X = (double*)malloc(sizeof(double) * r_train * (c_train+1));
	X_prices = (double*)malloc(sizeof(double) * r_train);

	int i, j, z;
	z=-1;
	
	for (i = 0; i < r_train && !feof(fp); i++){
		for (j = 0; j < c_train+2; j++){
			if (j == 0){
				X[i*(c_train+1)] = 1;
				z++;
			}
			else if (j == c_train+1){
				fscanf(fp, " %lf", &X_prices[i]);
			}
			else {
				fscanf(fp, " %lf", &X[i*c_train + j + z]);
			}
		}
	}


 	
 	FILE *fp2;
 	char data[10];
 	int r_data, c_data;
 	double* input;
 	
 	fp2 = fopen(argv[2], "r");
  	fscanf(fp2, "%4s", data);
  	fscanf(fp2, "%d", &c_data);
  	fscanf(fp2, "%d", &r_data);
  	
  	if (c_train != c_data){
  		printf("error");
  		return 1;
  	}
 	
 	input = (double*)malloc(sizeof(double) * r_data * (c_data+1));
 	
 	z=-1;
 	
 	for (i = 0; i < r_data && !feof(fp2); i++){
		for (j = 0; j < c_data+1; j++){
			if (j == 0){
				input[i*(c_data+1)] = 1;
				z++;
			}
			else {
				fscanf(fp2, " %lf", &input[i*c_data + j + z]);
			}
		}
	}
 
 	double* trans = (double*)malloc(sizeof(double) * r_train * (c_train+1));
 	double* mult = (double*)malloc(sizeof(double) * r_train * (r_train));
 	double* mult2 = (double*)malloc(sizeof(double) * r_train * (r_train));
 	double* W = (double*)malloc(sizeof(double) * r_train * (r_train));
 	double* inv = (double*)malloc(sizeof(double) * r_train * (r_train));
 	double* out = (double*)malloc(sizeof(double) * r_train * (r_train));
 	
 	//transpose X into X^T
 	transpose(X, r_train, (c_train+1), trans);
 	
 	//multiple X^T x X
 	multiply(trans, X, (c_train+1), r_train, r_train, (c_train+1), mult);
 	
 	//Calculate (X^T x X)^-1
 	inverse(mult, c_train+1, inv);
 	
 	//multiply X^T and Y
 	multiply(trans, X_prices, c_train+1, r_train, r_train, 1, mult2);
 	
 	//Calculate W
 	multiply(inv, mult2, c_train+1, c_train+1, c_train+1, 1, W);
 	
 	//multiply X_prices * W to produce an estimate for housing prices
 	multiply(input, W, r_data, c_data+1, c_train+1, 1, out);
 	
 	for (i = 0; i < r_data; i++){
 		printf("%.0f\n", out[i]);
 	}
 	
 	
 	free(X);
 	free(X_prices);
 	free(input);
 	free(trans);
 	free(mult);
 	free(mult2);
 	free(W);
 	free(inv);
 	free(out);
    
    return 0;
}
