#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

void multiply(int* matrix1, int* matrix2, int n, int* temp, int* out);
void printArray(int* array, int dim);
int* generateIdentity(int dim, int* temp);

int main(int argc, char *argv[])
{
if (argc < 2 || argv[1][0] == '\0'){
  printf("Error. Usage: ./mexp filename.txt/n");
  printf("Filename.txt must be a valid text file containing a matrix.");
  exit(0);
}

  int dim;
  FILE *fp;
  int n;
  int x;
  int i;
  int sum = 1;

  fp = fopen(argv[1], "r");
  fscanf(fp, "%d", &dim);

  if (dim == 1){
    fscanf(fp, " %d", &x);
    fscanf(fp, " %d", &n);
    for (i = 0; i < n; i++){
      sum = sum * x;
    }
    printf("%d\n", sum);
    exit(0);
  }

  int elements = (dim * dim);
  int *array = (int*)malloc(sizeof(int) * elements);
  int *temp = (int*)malloc(sizeof(int) * elements);
  int *out = (int*)malloc(sizeof(int) * elements);

  int j = 0;
  int k = 0;

  while (!feof(fp)){
      fscanf(fp, " %d", &array[k*dim + j]);
      j++;
      if (j % dim == 0 && j != 0){
            k++;
            j = 0;
      }
      if (k == dim){
          break;
      }
    }

  while (!feof(fp)){
    fscanf(fp, " %d", &n);
  }

  //handle the M^0 case:
  if (n < 0){
    printf("This program does not support negative exponents!\n");
    exit(0);
  }

  else if (n == 0){
    out = generateIdentity(dim, temp);
  }

  //Handle the M^1 case:

  else if (n == 1){
    out = array;
  }

  //Handle all other cases
  else {
  int firstP = 1;

  while (n != 1){
    if (firstP == 1){
      multiply(array, array, dim, out, temp);
      firstP = 0;
    }
    else{
      multiply(temp, array, dim, out, temp);
    }
      n--;
    }
}

  printArray(out, dim);

  fclose(fp);
  free(array);
  free(temp);
  free(out);

  return 0;
  }

void printArray(int* array, int dim){
  int i;
  int j;

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
    	if (j == (dim-1)){
    	printf("%d", array[i*dim + j]);
    	}
    	else{
      	printf("%d ", array[i*dim + j]);
      	}
    }
    if (i != (dim-1)){
    	printf("\n");
    }
  }

}

int* generateIdentity(int dim, int* temp){
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

void multiply(int* matrix1, int* matrix2, int dim, int* temp, int* out){

  int i;
  int j;
  int k;

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      temp[i*dim + j] = 0;
      for (k = 0; k < dim; k++){
        temp[i*dim + j] += matrix1[i*dim + k] * matrix2[k*dim + j];
        // printf("Insert %d at %d, %d by multiplying %d from matrix1 (%d, %d) and %d from matrix2 (%d, %d)\n", matrix1[i][k] * matrix2[k][j], i, j, matrix1[i][k], i, k, matrix2[k][j], k, j);
      }

    }
  }

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      out[i*dim + j] = temp[i*dim + j];
    }
  }

}


