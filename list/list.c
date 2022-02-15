#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

struct node* createNode(int data);
void freeList(struct node* head);
void printList(struct node* head);
void delete(struct node** head, int Data);
void insert(struct node** head, struct node* temp);
int search(struct node* head, int data);

int size;

struct node {
   int data;
   struct node* next;
};

struct node* createNode(int data){
  struct node* temp;
  temp = (struct node*)malloc(sizeof(struct node));
  temp->next = NULL;
  temp->data = data;
  return temp;
}

int search(struct node *head, int data){
struct node *p = head;

while (p != NULL){
	if (p->data == data){
		return 1;
	}
	p = p->next;
}

return 0;

}

void insert(struct node **head, struct node* temp){
  struct node* p;
  // printf("Input: %d\n", inData);
  // printf("tempData: %d\n", temp->data);

  if (*head == NULL || (*head)->data > temp->data){
        size++;
        temp->next = *head;
        *head = temp;
        return;
  }

  if ((*head)->data == temp->data){
	free(temp);
    return;
  }

  p = *head;
  while (p->next != NULL && p->next->data < temp->data) {
      p = p->next;
  }

  if (p->next == NULL){
    if (p->data == temp->data){
      return;
    }
  }
  else {
    if (p->next->data == temp->data){
    return;
    }
  }

  temp->next = p->next;
  p->next = temp;
  size++;
  return;
}

void delete(struct node** head, int Data){
  struct node* temp;
  struct node* p;

  p = *head;

  if (*head == NULL){
    return;
  }
  else if ((*head)->data == Data){
	temp = *head;
    *head = p->next;
    size--;
	free(temp);
    return;
  }
  else{
    while (p->next != NULL){
      if (p->next->data == Data){
        temp = p->next;
        p->next = p->next->next;
        size--;
        free(temp);
        return;
      }
      else {
        p = p->next;
      }
    }
  }
  return;
}

void printList(struct node* head){
  struct node *p;
  p = head;
  if (size == 0){
printf("%d :", size);
printf("\n");
return;
}
else{
  printf("%d : ", size);

  while (p)
  {
	if (p->next == NULL){
	printf("%d", p->data);
	p = p->next;
	}
	else {
      printf("%d ", p->data);
      p = p->next;
	}
  }

  printf("\n");
}
}

void freeList(struct node* head){
struct node* n = head;

while (n != NULL){
struct node *temp = n;
n = n->next;
free(temp);
}

}

int main(int argc, char *argv[]){
  char ch;
  int data;
  struct node *head = NULL;
  //int first2 = 1;
	//ch2!=EOF
  while (!feof(stdin) && scanf(" %c %d", &ch, &data) == 2){
   // printf("ch: %c, data: %d\n", ch, data);
    if (ch == 'i'){
	if (search(head, data) == 0){
      		insert(&head, createNode(data));
	}
      printList(head);
    }
    else if (ch == 'd'){
      delete(&head, data);
      printList(head);
    }
   // while ((ch2 = fgetc(stdin)) == '\n');
   // ungetc(ch2, stdin);
  }

freeList(head);
return 0;

}
