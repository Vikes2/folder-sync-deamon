#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node 
{
  char* fileName;
  int fileType;
  struct node * next;
} Node;

typedef struct list 
{
  Node * head; 
} List;

List * emptylist();
void add(char* _fileName, int _fileType, List * list);

int valueExists(char* _fileName, int _fileType, List * list);

void display(List * list);
void deleteElement(char* _fileName, List* list);
Node * popElement(List * list);

void destroy(List * list);

#endif