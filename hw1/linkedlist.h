#ifndef linkedlist_h
#define linkedlist_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[10];
  char firstName[30]; //Ten
  char lastName[30]; //Ho va dem
  float midTermScore;
  float finallTermScore;
  char charScore;
} Student;

typedef struct Node_{
  Student *data;
  struct Node_ *next;
} Node;
/*-----------------------------------------------------------------*/
int compElement(Student *e1, Student *e2);
Node *makeNode(Student x);
void Full();
int isEmpty(Node *top);
void addHead(Node **top, Node *newNode);
void addLast(Node **top, Node *newNode);
void delHead(Node **top);
void delMid(Node *prev);
void addSort(Node **top, Node *newNode);
void delNode(Node **top, char* s);
Node* searchNode(Node* top, char* s);
void freeNode(Node *node);
void freeList(Node **top);
/*-----------------------------------------------------------------*/
int compElement(Student *e1, Student *e2) {
  if (strcmp(e1->firstName, e2->firstName) < 0){
    return -1;
  } else if (strcmp(e1->firstName, e2->firstName) > 0){
    return 1;
  } else if (strcmp(e1->lastName, e2->lastName) < 0){
    //e1->firstName == e2->firstName
    return -1;
  } else if (strcmp(e1->lastName, e2->lastName) > 0){
    //e1->firstName == e2->firstName
    return 1;
  } else {
    //e1->firstName == e2->firstName
    //e1->lastName == e2->lastName
    return 0;
  }
}
/*-----------------------------------------------------------------*/
Node *makeNode(Student x) {
  Node *node;
  node = (Node *)malloc(sizeof(Node));
  node->data = (Student*)malloc(sizeof(Student));
  if (node == NULL || node->data == NULL) {
    Full();
    return NULL;
  }

  strcpy(node->data->id, x.id);
  strcpy(node->data->firstName, x.firstName);
  strcpy(node->data->lastName, x.lastName);
  node->data->midTermScore = x.midTermScore;
  node->data->finallTermScore = x.finallTermScore;
  node->data->charScore = x.charScore;
  node->next = NULL;
  return node;
}
/*-----------------------------------------------------------------*/
void Full() {
  printf("NO MEMORY!\n");
}

int isEmpty(Node *top) {
  return (top == NULL);
}
/*-----------------------------------------------------------------*/
void addHead(Node **top, Node *newNode) {
  newNode->next = (*top);
  (*top) = newNode;
}
/*-----------------------------------------------------------------*/
void addLast(Node **top, Node *newNode) {
  Node *node;
  if (isEmpty(*top)){
    addHead(top, newNode);
  } else {
    for (node=(*top); node->next!=NULL; node=node->next);
    node->next = newNode;
  }
}
/*-----------------------------------------------------------------*/
void delHead(Node **top) {
  Node *node;
  node = (*top);
  (*top) = node->next;
  freeNode(node);
}
/*-----------------------------------------------------------------*/
void delMid(Node *prev) {
  Node *node;
  node = prev->next;
  prev->next = node->next;
  freeNode(node);
}
/*-----------------------------------------------------------------*/
void addSort(Node **top, Node *newNode) {
  if (isEmpty(*top)){
    addHead(top, newNode);
  } else {
    Node *sau,*trc;

    sau=(*top);
    trc=(*top);
    if (compElement(newNode->data,sau->data) < 0){
       addHead(top, newNode);
    } else {
        while ( (sau!=NULL) && (compElement(sau->data,newNode->data)!=1))
        {
              trc=sau;
              sau=sau->next;
        }
        trc->next=newNode;
        newNode->next=sau;
     }
  }
}
/*-----------------------------------------------------------------*/
void delNode(Node** top, char* s){
  Node *temp;
  Node *p;
  if (strcmp((*top)->data->id, s) == 0){
    temp = (*top);
    (*top) = (*top)->next;
    freeNode(temp);
  } else{
    for (p = (*top); p->next!=NULL; p=p->next){
      if (strcmp(p->next->data->id, s)==0){
        temp = p->next;
        p->next = temp->next;
        freeNode(temp);
        break;
      }
    }
  }
}
/*-----------------------------------------------------------------*/
Node* searchNode(Node *top, char* s){
  Node *p;
  for (p=top; p!=NULL; p=p->next){
    if (strcmp(p->data->id, s)==0){
      return p;
    }
  }
  return NULL;
}
/*-----------------------------------------------------------------*/
void freeNode(Node* node){
  free(node->data);
  free(node);
}
/*-----------------------------------------------------------------*/
void freeList(Node** top){
  while (!isEmpty(*top)){
    delHead(top);
  }
}


#endif