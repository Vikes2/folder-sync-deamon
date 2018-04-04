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

int valueExists(const char* _fileName, List * list);

void display(List * list);

void destroy(List * list);

 