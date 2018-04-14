#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node 
{
  char *fileName;
  int fileType;
  struct node *next;
} Node;

typedef struct list 
{
  Node *head; 
} List;

/** 
 * [emptyList]
 * @brief Inits an empty list 
 * @retval list
 */
List *emptyList();

/** 
 * [createNode]
 * @brief  creates a new element which consist of fileName and fileType, allocate required memory.
 * @param  fileName: char *
 * @param  fileType:  char *
 * @retval  Node * - the created element.
 */
Node *createNode(char *fileName, int fileType);

/** 
 * [add]
 * @brief  Adds a new element to the specified list.
 * @param  _fileName: char *
 * @param  _fileType: char *
 * @param  list: List *
 * @retval None
 */
void add(char *_fileName, int _fileType, List *list);

/** 
 * [valueExists]
 * @brief  Checks whether the specified file name and file type exists. 
 * @param  _fileName: char *
 * @param  _fileType: char *
 * @param  list: List *
 * @retval 1 - if exists, either returns 0
 */
int valueExists(char *_fileName, int _fileType, List *list);

/** 
 * @TO DO:
 * REMOVE THIS USELESS FUNCTION
 */
void display(List *list);

/** 
 * [deleteElement]
 * @brief  Deletes the element from the list
 * @param  _fileName: char *
 * @param  list: List *
 * @retval None
 */
void deleteElement(char *_fileName, List *list);

/** 
 * [popElement]
 * @brief  Gets the first element of the list
 * @param  list: List *
 * @retval Node * first element or NULL if the list doesn't contain any items.
 */
Node *popElement(List *list);

/** 
 * [destroy]
 * @brief  Destroys the list.
 * @param  list: List *
 * @retval None
 */
void destroy(List **list);

#endif