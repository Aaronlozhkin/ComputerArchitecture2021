#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

int top = -1;

int push(char *array, char ch);
char pop(char *array);

int main(int argc, char *argv[])
{
if (argc < 2 || argv[1][0] == '\0'){
exit(0);
}

int length = strlen(argv[1])+1;
char *array = malloc(sizeof(char) * length);
int i;
char ch;

for (i = 0; i < length-1; i++){
if (argv[1][i] == '{' || argv[1][i] == '(' || argv[1][i] == '['){
top = push(array, argv[1][i]);
// printf("Pushed %c successfully\n", array[top]);
}
else if (argv[1][i] == '}'){
ch = pop(array);
// printf("%c was popped successfully", ch);
if (ch != '{') {
printf("%d: %c\n", i, argv[1][i]);
free(array);
exit(1);
}
}
else if (argv[1][i] == ']'){
ch = pop(array);
if (ch != '['){
printf("%d: %c\n", i, argv[1][i]);
free(array);
exit(1);
}

}
else if (argv[1][i] == ')'){
ch = pop(array);
if (ch != '('){
printf("%d: %c\n",  i, argv[1][i]);
free(array);
exit(1);
}
}
}

if (top != -1){
printf("open: ");

while (top != -1){
ch = pop(array);

if (ch == '('){
printf(")");
}
else if (ch == '{'){
printf("}");
}
else if (ch == '['){
printf("]");
}
}

printf("\n");
exit(1);
}

free(array);
exit(0);
return 0;
}

int push(char *array, char ch){

top = top + 1;
array[top] =  ch;

return top;

}

char pop(char *array){
if (top == -1){
return 'a';
}
char data = array[top];
top = top - 1;
return data;

}
