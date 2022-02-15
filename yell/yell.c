#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
if (argc < 2 || argv[1][0] == '\0'){
return 0;
}

int i;

for (i = 0; i < strlen(argv[1]); i++){
printf("%c", toupper(argv[1][i]));
}



printf("!!\n");
return 0;
}


