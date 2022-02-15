#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include <string.h>

int containsNumber(char* arg, int length);

int main(int argc, char *argv[])
{
	
	if (argc < 2 || argv[1][0] == '\0'){
		exit(0);
	}
	
	int length = strlen(argv[1]) + 1;
	
	if (containsNumber(argv[1], length) == 1){
		printf("error");
		return 0;
	}
	
	
	
	int *intarray = (int*)malloc((length/2 + 1) * sizeof(int));
	char* array = malloc(sizeof(char) * (length/2 + 1));
	char* out = calloc(length, sizeof(char));

	int dynamiclength = 0;
	int argvPointer = 0;
	int counter;
	int isCompressedBigger = 0;
	
	if (length-1 == 1){
		isCompressedBigger = 1;
	}
	
	while ((argvPointer+1) < (length)){
		
		counter = 1;	
		
		//printf("%d , %c", argvPointer, argv[1][argvPointer]);
			
		while (argv[1][argvPointer] == argv[1][argvPointer + 1]){
			counter++;
			argvPointer++;
		}
		
		array[dynamiclength] = argv[1][argvPointer];
		intarray[dynamiclength] = counter;
		
		dynamiclength++;
		argvPointer++;
		
		if (dynamiclength*2 > (length-1) || argv[1][argvPointer+1] == '\n'){
			isCompressedBigger = 1;
			free(intarray);
			free(array);
			break;
		}
		
	}
	
	int i;
	
	if (isCompressedBigger == 0){
		
		for (i = 0; i < dynamiclength; i++){
			printf("%c", array[i]);
			printf("%d", intarray[i]);
		}
		free(intarray);
		free(array);
	
	}
	else {
		strncpy(out, argv[1], length-1);
		printf("%s", out);
	}

	printf("\n");
	exit(0);
	return 0;
}

int containsNumber(char *arg, int length){
	int j;
	for (j = 0; j < length; j++){
		if (isdigit(arg[j])){
			return 1;
		}
	}
	
	return 0;
}
