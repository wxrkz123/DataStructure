#pragma once
#include <stdbool.h>
#include <stddef.h>

// --- Opaque Pointer Type ---
// 用户只能看到这些类型的指针，不能直接访问其内部结构 ―― OOP封装

typedef struct DoublyLinkedList DoublyLinkedList;
typedef struct DListNode DListNode;

// --- Callback Function Pointer Typedefs ---

// 定义用户需要提供的回调函数类型，增强代码可读性

typedef int (*CompareFunc)(const void* data1, const void* data2);
typedef void (*PrintFunc)(const void* data);
typedef void (*FreeFunc)(void* data);
typedef void (*ActionFunc)(void* data, void* context);

// MemoryPool 相关函数指针类型
// 内存池结构。它预先分配一块内存，用于存储链表节点，减少频繁的内存分配和释放操作。
// 并且将其划分为多个固定大小的块，以便快速分配和释放。内部通过一个单向链表来管理这些内存块。



// API FUNCTION DECLARATIONS

/**
 * @brief 创建一个新的双向链表
 * 
 * @param initial_capacity 内存池初始容量（可用于预分配节点或内部结构）
 * @param free_func 用户自定义的释放数据的回调函数，链表销毁或节点删除时调用
 * @return DoublyLinkedList* 新创建的链表指针，失败时返回NULL
 * 
 * @note 创建后需调用相应的销毁函数释放链表资源
 */
DoublyLinkedList* List_Create(size_t initial_capacity, FreeFunc free_func);

void List_Destroy(DoublyLinkedList** list);

bool List_Append(DoublyLinkedList* list, const void* data);

bool List_Prepend(DoublyLinkedList* list, const void* data);

bool List_InsertAfter(DoublyLinkedList* list, DListNode* node, const void* data);

void List_DeleteNode(DoublyLinkedList* list, DListNode* node);

DListNode* List_Find(DoublyLinkedList* list, const void* data_to_find, CompareFunc compare_func);

size_t List_GetSize(const DoublyLinkedList* list);

void List_ForEach(const DoublyLinkedList* list, ActionFunc action_func, void* context);

void* List_GetData(const DListNode* node);