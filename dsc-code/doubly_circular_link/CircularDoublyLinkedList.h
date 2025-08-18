#pragma once

#include <stddef.h>  
#include <stdbool.h>  
#include <threads.h>  


// Internal data structure for the doubly linked list
typedef struct CircularDoublyLinkedList CircularDoublyLinkedList;
typedef struct CDListNode CDListNode;

typedef void (*FreeFunc)(void*);
typedef int (*CompareFunc)(const void*, const void*);
typedef void (*ActionFunc)(void*, void*);


CDListNode* List_GetHeadNode(CircularDoublyLinkedList* list);

CircularDoublyLinkedList* List_Create(size_t initial_capacity, FreeFunc free_func);
void List_Destroy(CircularDoublyLinkedList** list_ptr);
bool List_Append(CircularDoublyLinkedList* list, void* data);
bool List_Prepend(CircularDoublyLinkedList* list, void* data);
void List_DeleteNode(CircularDoublyLinkedList* list, CDListNode* node);
CDListNode* List_Find(CircularDoublyLinkedList* list, const void* data_to_find, CompareFunc compare_func);
void List_RotateForward(CircularDoublyLinkedList* list);
void List_RotateBackward(CircularDoublyLinkedList* list);
size_t List_GetSize(const CircularDoublyLinkedList* list);
void List_ForEach(CircularDoublyLinkedList* list, ActionFunc action_func, void* context);
void* CDListNode_GetData(const CDListNode* node);
