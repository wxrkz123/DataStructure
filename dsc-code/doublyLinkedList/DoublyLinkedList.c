#include "DoublyLinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <errno.h>
// Windows -> Windows.h Linux/MacOS -> thread.h
//#include <Windows.h>




struct DListNode {
	void* data;               // Pointer to the data stored in the node
	struct DListNode* next;   // Pointer to the next node in the list
	struct DListNode* prev;   // Pointer to the previous node in the list
};

// 内存池 高效地管理链表节点。（DListNode）
typedef struct MemoryPool {
	DListNode* pool_memory; // Pointer to the memory pool
	DListNode* free_list_head;   // Pointer to the list of free nodes
} MemoryPool;

struct DoublyLinkedList {
	DListNode* head;          // Pointer to the first node in the list
	DListNode* tail;          // Pointer to the last node in the list
	size_t size;              // Number of nodes in the list
	mtx_t lock;				// Mutex for thread safety 互斥锁，确保多线程环境下的安全访问
	MemoryPool* pool;   // Memory pool for managing nodes
	FreeFunc user_free_func;       // User-defined function to free data
};

// Function to create a new memory pool

static MemoryPool* MemoryPool_Create(size_t initial_capacity) {

	if (initial_capacity == 0) {
		perror("Invalid initial capacity for memory pool");
		return NULL; // Invalid capacity
	}
	MemoryPool* pool = (MemoryPool*)malloc(sizeof(MemoryPool));
	if (!pool) {
		perror("Failed to allocate memory for memory pool");
		return NULL; // Memory allocation failed
	}
	// core
	pool->pool_memory = (DListNode*)malloc(initial_capacity * sizeof(DListNode));
	if (!pool->pool_memory) {
		perror("Failed to allocate memory for pool nodes");
		free(pool);
		return NULL; // Memory allocation failed
	}

	// 初始化空闲列表
	pool->free_list_head = &pool->pool_memory[0];

	// 修复缓冲区溢出问题  
		// 确保在循环中不越界访问内存  
	
	for (size_t i = 0; i < initial_capacity - 1; ++i) {
		pool->pool_memory[i].next = &pool->pool_memory[i + 1];
	}
	if(initial_capacity > 0){
		pool->pool_memory[initial_capacity - 1].data = NULL; // Initialize the last node's data to NULL
	}

	return pool;
}

static void MemoryPool_Destroy(MemoryPool* pool) {
	if (!pool) return;
	free(pool->pool_memory);
	free(pool);
}

static DListNode* MemoryPool_AllocateNode(MemoryPool* pool) {
	if (!pool || !pool->free_list_head) {
		return NULL; // No available nodes
	}
	DListNode* node = pool->free_list_head;

	// Move the head of the free list to the next node
	pool->free_list_head = node->next;

	return node;
}


static void MemoryPool_FreeNode(MemoryPool* pool, DListNode* node) {
	if (!pool || !node) return;
	// 将节点添加回空闲列表
	// 将要释放的节点的 next 指向当前空闲列表的头节点
	node->next = pool->free_list_head;
	// 更新空闲列表的头节点为当前释放的节点
	pool->free_list_head = node;
}

// 实例的工厂函数（设计模式）
DoublyLinkedList* List_Create(size_t initial_capacity, FreeFunc free_func) {
	if (initial_capacity == 0) {
		perror("Invalid initial capacity for doubly linked list");
		return NULL; // Invalid capacity
	}
	DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
	if (!list) {
		perror("Failed to allocate memory for doubly linked list");
		return NULL; // Memory allocation failed
	}

	list->pool = MemoryPool_Create(initial_capacity);
	if (!list->pool) {
		free(list);
		return NULL; // Memory pool creation failed
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->user_free_func = free_func;


	// Initialize the mutex for thread safety
	if (mtx_init(&list->lock, mtx_plain) != thrd_success) {
		perror("Failed to initialize mutex");
		MemoryPool_Destroy(list->pool);
		free(list);
		return NULL; // Mutex initialization failed
	}

	return list;
}

void List_Destroy(DoublyLinkedList** list_ptr) {
	if (!list_ptr || !*list_ptr) return;

	DoublyLinkedList* list = *list_ptr; // Dereference the pointer to get the list	

	mtx_lock(&list->lock); // Lock the list for thread safety

	DListNode* current = list->head;
	while (current != NULL) {
		DListNode* next_node = current->next;
		if (list->user_free_func && current->data) {
			list->user_free_func(current->data); // Call user-defined free function
		}

		current = next_node;
	}

	MemoryPool_Destroy(list->pool); // Destroy the memory pool
	mtx_unlock(&list->lock); // Unlock the list
	mtx_destroy(&list->lock); // Destroy the mutex
	free(list); // Free the list structure	
	*list_ptr = NULL; // Set the pointer to NULL to avoid dangling pointer
}

// 内部辅助函数：创建一个新的节点 helper function
static DListNode* _internal_CreateNode(DoublyLinkedList* list, const void* data) {
	if (!list || !list->pool) {
		return NULL; // Invalid list or memory pool
	}
	DListNode* new_node = MemoryPool_AllocateNode(list->pool);
	if (!new_node) {
		perror("Failed to allocate node from memory pool");
		return NULL; // Memory allocation failed
	}
	new_node->data = data; // Store the data pointer
	new_node->next = NULL; // Initialize next pointer to NULL ，链接到其他节点的操作由调用者完成
	new_node->prev = NULL;
	return new_node;
}

bool List_Append(DoublyLinkedList* list, const void* data) {
	if (!list || !data) {
		return false; // Invalid list or data
	}

	mtx_lock(&list->lock); // Lock the list for thread safety

	DListNode* new_node = _internal_CreateNode(list, data);
	if (!new_node) {
		mtx_unlock(&list->lock); // Unlock before returning
		return false; // Node creation failed
	}

	if (list->tail == NULL) {
		list->head = new_node; // If the list is empty, set head to the new node
		list->tail = new_node; // Set tail to the new node
	}
	else {
		list->tail->next = new_node; // Link the new node to the end of the list
		new_node->prev = list->tail; // Set the previous pointer of the new node
		list->tail = new_node; // Update tail to the new node
	}
	list->size++;
	mtx_unlock(&list->lock); // Unlock the list
	return true;
}

bool List_Prepend(DoublyLinkedList* list, const void* data) {
	if (!list || !data) {
		return false; // Invalid list or data
	}
	mtx_lock(&list->lock); // Lock the list for thread safety

	DListNode* new_node = _internal_CreateNode(list, data);

	if (!new_node) {
		mtx_unlock(&list->lock); // Unlock before returning
		return false; // Node creation failed
	}

	if (list->head == NULL) {
		list->head = new_node; // If the list is empty, set head to the new node
		list->tail = new_node; // Set tail to the new node
	}
	else {
		new_node->next = list->head; // Link the new node to the start of the list
		list->head->prev = new_node; // Set the previous pointer of the old head
		list->head = new_node; // Update head to the new node
	}
	list->size++;

	mtx_unlock(&list->lock); // Unlock the list

	return true;
}

void List_DeleteNode(DoublyLinkedList* list, DListNode* node) {
	if (!list || !node) {
		return; // Invalid list or node
	}
	mtx_lock(&list->lock); // Lock the list for thread safety

	if (node->prev) {
		node->prev->next = node->next; // Link previous node to next node
	}
	else {
		list->head = node->next; // If deleting head, update head
	}

	if (node->next) {
		node->next->prev = node->prev; // Link next node to previous node
	}
	else {
		list->tail = node->prev; // If deleting tail, update tail
	}

	if (list->user_free_func && node->data) {
		list->user_free_func(node->data); // Call user-defined free function
	}

	MemoryPool_FreeNode(list->pool, node); // Free the node back to the memory pool
	list->size--;

	mtx_unlock(&list->lock); // Unlock the list
}

DListNode* List_Find(DoublyLinkedList* list, const void* data_to_find, CompareFunc compare_func) {
	if (!list || !data_to_find || !compare_func) {
		return NULL; // Invalid list, data, or compare function
	}
	mtx_lock(&list->lock); // Lock the list for thread safety

	DListNode* current = list->head;
	while (current != NULL) {
		if (compare_func(current->data, data_to_find) == 0) {
			mtx_unlock(&list->lock); // Unlock before returning
			return current; // Found the node
		}
		current = current->next; // Move to the next node
	}
	mtx_unlock(&list->lock); // Unlock the list
	return NULL; // Node not found
}

size_t List_GetSize(const DoublyLinkedList* list) {
	if (!list) {
		return 0; // Invalid list
	}
	return list->size; // Return the size of the list
}

void List_ForEach(const DoublyLinkedList* list, ActionFunc action_func, void* context) {
	if (!list || !action_func) {
		return; // Invalid list or action function
	}
	mtx_lock(&list->lock); // Lock the list for thread safety
	DListNode* current = list->head;
	while (current != NULL) {
		action_func(current->data, context); // Call the action function on each node's data
		current = current->next; // Move to the next node
	}
	mtx_unlock(&list->lock); // Unlock the list
}

void* List_GetData(const DListNode* node) {
	return node ? node->data : NULL; // Return the data pointer from the node
}

bool List_InsertAfter(DoublyLinkedList* list, DListNode* node, const void* data)
{
	if (!list || !node || !data) {
		return false; // Invalid list, node, or data
	}
	mtx_lock(&list->lock); // Lock the list for thread safety
	DListNode* new_node = _internal_CreateNode(list, data);
	if (!new_node) {
		mtx_unlock(&list->lock); // Unlock before returning
		return false; // Node creation failed
	}
	new_node->next = node->next; // Link the new node to the next node
	new_node->prev = node; // Link the new node to the current node
	if (node->next) {
		node->next->prev = new_node; // Update the next node's previous pointer
	}
	else {
		list->tail = new_node; // If inserting at the end, update tail
	}
	node->next = new_node; // Update the current node's next pointer
	list->size++; // Increase the size of the list
	mtx_unlock(&list->lock); // Unlock the list
	return true; // Successfully inserted the new node
}
