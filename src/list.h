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

/**
 * [add description]
 * Test
 * @param {char *} _fileName test1
 * @param {int} _fileType test2
 * @param {List *} list test3
 */
void add(char* _fileName, int _fileType, List * list);

int valueExists(char* _fileName, int _fileType, List * list);

void display(List * list);
void deleteElement(char* _fileName, List* list);
Node * popElement(List * list);

void destroy(List * list);

#endif