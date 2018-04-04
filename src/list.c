#include "list.h"

Node * createnode( char* fileName, int fileType);

Node * createnode(char* _fileName, int _fileType)
{
  Node * newNode = malloc(sizeof(Node));
  newNode->fileName = _fileName;
  newNode->fileType = _fileType;
  newNode->next = NULL;
  return newNode;
}

List * emptylist()
{
  List * list = malloc(sizeof(List));
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
    list->head = createnode(_fileName, _fileType);
  }
  else 
  {
    current = list->head; 
    while (current->next!=NULL)
    {
      current = current->next;
    }
    current->next = createnode(_fileName, _fileType);
  }
}

int valueExists(const char* _fileName, List * list)
{
  Node * current = list->head;
  if(list->head == NULL)
    return 0;
  while(current->next != NULL)
  {
    if(strcmp(current->fileName,_fileName) ==0)
      return 1;
    current= current->next;
  }
  if(strcmp(current->fileName,_fileName) ==0)
      return 1;

  return 0;
}

void destroy(List * list)
{
  Node * current = list->head;
  Node * next = current;
  while(current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }

   free(list);

}