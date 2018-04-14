#include "list.h"

Node *createNode(char *_fileName, int _fileType)
{
  Node *newNode = malloc(sizeof(Node));
  newNode->fileName = _fileName;
  newNode->fileType = _fileType;
  newNode->next = NULL;
  return newNode;
}

List *emptyList()
{
  List *list = malloc(sizeof(List));
  list->head = NULL;
  return list;
}

void display(List * list) 
{
  if(list == NULL)
    return;
  Node * current = list->head;
  if(list->head == NULL) 
    return;

  while(current->next != NULL)
  {
    printf("%s %d, ", current->fileName, current->fileType);
    current = current->next;
  }
  printf("%s %d, ", current->fileName, current->fileType);
}

void add(char* _fileName, int _fileType, List * list)
{
  Node * current = NULL;
  if(list->head == NULL)
  {
    list->head = createNode(_fileName, _fileType);
  }
  else 
  {
    current = list->head; 
    while (current->next!=NULL)
    {
      current = current->next;
    }
    current->next = createNode(_fileName, _fileType);
  }
}

void deleteElement(char* _fileName, List* list)
{
  if(list == NULL || list->head == NULL)
  {
    return;
  }
  Node * current = list->head;
  Node * previous = current;
  if(strcmp(current->fileName,_fileName) == 0)
  {
    list->head = current->next;
    free(current);
    return;
  }
  current= current->next;
  while(current != NULL)
  {
     if(strcmp(current->fileName,_fileName) == 0)
    {
      previous->next=current->next;
      free(current);
      return;
    }
    previous=current;
    current=current->next;
  }
}


int valueExists(char* _fileName, int _fileType, List * list)
{
  Node * current = list->head;
  if(list->head == NULL)
    return 0;
  while(current != NULL)
  {
    if(strcmp(current->fileName,_fileName) == 0 && current->fileType == _fileType)
      return 1;
    current= current->next;
  }

  return 0;
}

void destroy(List **list)
{
  Node *current = (*list)->head;
  Node *next = current;
  while(current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }
  free(*list);
  *list = NULL;
}


Node * popElement(List * list)
{
  if(list->head == NULL)
    return NULL;
  Node * result = createNode(list->head->fileName,list->head->fileType);
  Node * elem = list->head;
  list->head = elem->next;
  free(elem);
  return result;
  
}
