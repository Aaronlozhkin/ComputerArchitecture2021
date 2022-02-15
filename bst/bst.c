#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
   int data;
   struct node* left;
struct node* right;
};

void search(struct node* head, int n);
int int_search(struct node* head, int n);
struct node* insert(struct node* head, int data, int counter);
struct node* delete_help(struct node* head, int n);
void print(struct node* head);
void freeNodes(struct node* head);

struct node* createNode(int data){
  struct node* temp;
  temp = (struct node*)malloc(sizeof(struct node));
  temp->left = NULL;
temp->right = NULL;
  temp->data = data;
  return temp;
}

void search(struct node* head, int n){
int x = int_search(head,n);
if (x == 1){
printf("present");
}
else {
printf("absent");
}
}

int int_search(struct node* head, int n){

if (head == NULL){
return 0;
}

if (head->data == n){
return 1;
}
else if (n < head->data){
return int_search(head->left, n);
}
else{
return int_search(head->right, n);
}
}

struct node* insert(struct node* head, int data, int counter){

if (head == NULL){
printf("inserted");
return createNode(data);
}

if (data < head->data){
head->left = insert(head->left, data, counter++);
}
else if (data > head->data){
head->right = insert(head->right, data, counter++);
}
else {
printf("not inserted");
return head;
}

if (counter == 0){
printf("inserted");
}

return head;
}

struct node* delete_help(struct node* head, int n){
if (head == NULL){
return head;
}

if (head->data < n){
head->right = delete_help(head->right, n);
}
else if (head->data > n){
head->left = delete_help(head->left, n);
}
else{
if (head->data == n && head->left == NULL && head->right==NULL){
free(head);
return NULL;
}
else if (head->left == NULL){
struct node* temp = head->right;
free(head);
return temp;
}
else if (head->right == NULL){
struct node* temp = head->left;
free(head);
return temp;
}

struct node* temp = head->left;

while (temp->right != NULL){
temp = temp->right;
}

head->data = temp->data;

head->left = delete_help(head->left, temp->data);

}

return head;

}

void print(struct node* head){
if (head == NULL){
return;
}
else {
printf("(");
if (head->left != NULL){
print(head->left);
}
printf("%d", head->data);
if (head->right != NULL){
print(head->right);
}
printf(")");
}
}

void freeNodes(struct node* head){
	if (head == NULL){
	return;
	}
	
	freeNodes(head->left);
	freeNodes(head->right);
	free(head);
}

int main(int argc, char *argv[])
{
char ch;
  int data;
  struct node *head = NULL;
int first = 1;

printf("Commands:\n");
printf("i x: inserts x into bst\n");
printf("d x: deletes x from bst if present\n");
printf("s x: searches bst for x\n");
printf("p: prints the in-order traversal of the bst\n");

 while (!feof(stdin) && scanf(" %c", &ch) == 1){
    // printf("ch: %c, data: %d\n", ch, data);
    if (ch == 'i'){
    scanf(" %d", &data);
if (first == 1){
head = insert(head, data, 0);
first = 0;
}
else{
insert(head, data, 0);
}
    }
    else if (ch == 'd'){
    scanf(" %d", &data);
int x = int_search(head, data);
if (x == 0){
printf("absent");
}
else {
printf("deleted");
head = delete_help(head, data);
if (head == NULL){
first = 1;
}
}
    }
else if (ch == 's'){
scanf(" %d", &data);
      search(head, data);
    }
else if (ch == 'p'){
      print(head);
    }
    else if (ch == '^'){
      exit(0);
    }

    printf("\n");

  }
  
  freeNodes(head);

return 0;
}
