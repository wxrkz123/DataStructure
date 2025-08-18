#define _CRT_SECURE_NO_WARNINGS
#include "CircularDoublyLinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <errno.h>
// Windows -> Windows.h Linux/MacOS -> thread.h
//#include <Windows.h>



// Internal structure of the circular doubly linked list
struct CDListNode {
	void* data;
	CDListNode* next;
	CDListNode* prev;
};

typedef struct MemoryPool {
	CDListNode* pool_memory;
	CDListNode* free_list_head;
} MemoryPool;


struct CircularDoublyLinkedList {
	CDListNode* head; // Pointer to the head of the list
	size_t size; // Number of elements in the list
	mtx_t lock; // Mutex for thread safety
	MemoryPool* pool; // Memory pool for node allocation
	FreeFunc user_free_func; // User-defined function to free data
};

static MemoryPool* MemoryPool_Create(size_t initial_capacity) {

	if (initial_capacity == 0) {
		return NULL; // Invalid initial capacity
	}
	MemoryPool* pool = malloc(sizeof(MemoryPool));
	if (!pool) {
		return NULL;
	}

	pool->free_list_head = NULL;


	pool->pool_memory = malloc(initial_capacity * sizeof(CDListNode));
	if (!pool->pool_memory) {
		free(pool);
		return NULL;
	}

	// Important: Initialize the pool memory
	pool->free_list_head = &pool->pool_memory[0]; // Initialize the free list head

	for (size_t i = 0; i < initial_capacity - 1; ++i) {
		pool->pool_memory[i].next = &pool->pool_memory[i + 1];
	}
	pool->pool_memory[initial_capacity - 1].next = NULL;

	return pool;
}

static void MemoryPool_Destroy(MemoryPool* pool) {
	if (!pool) {
		return;
	}
	free(pool->pool_memory);
	free(pool);
}

static CDListNode* MemoryPool_AllocateNode(MemoryPool* pool) {
	if (!pool->free_list_head) {
		return NULL; // No free nodes available
	}
	CDListNode* node = pool->free_list_head;
	pool->free_list_head = node->next;
	return node;
}

static void MemoryPool_FreeNode(MemoryPool* pool, CDListNode* node) {

	if (!pool || !node) {
		return; // Invalid pool or node
	}

	node->data = NULL; // Clear the data pointer
	node->next = NULL; // Clear the next pointer

	node->next = pool->free_list_head;
	pool->free_list_head = node;
}

CircularDoublyLinkedList* List_Create(size_t initial_capacity, FreeFunc free_func) {
	CircularDoublyLinkedList* list = malloc(sizeof(CircularDoublyLinkedList));
	if (!list) {
		return NULL;
	}

	list->pool = MemoryPool_Create(initial_capacity);
	if (!list->pool) {
		free(list);
		return NULL;
	}

	list->head = NULL;
	list->size = 0;
	list->user_free_func = free_func;

	if (mtx_init(&list->lock, mtx_plain) != thrd_success) {
		MemoryPool_Destroy(list->pool);
		free(list);
		return NULL;
	}
	return list;
}

void List_Destroy(CircularDoublyLinkedList** list_ptr) {
	if (!list_ptr || !*list_ptr) {
		return;
	}
	CircularDoublyLinkedList* list = *list_ptr;

	mtx_lock(&list->lock);

	if (list->head != NULL) {
		CDListNode* current = list->head;
		for (size_t i = 0; i < list->size; i++)
		{
			CDListNode* next_node = current->next;
			if (list->user_free_func && current->data) {
				list->user_free_func(current->data);
			}
			current = next_node;
		}
	}

	MemoryPool_Destroy(list->pool);
	mtx_unlock(&list->lock);
	mtx_destroy(&list->lock);
	
	free(list);
	*list_ptr = NULL;

	// List_Destroy(&list_ptr);
}


bool List_Append(CircularDoublyLinkedList* list, void* data) {
	if (!list) {
		return false;
	}
	mtx_lock(&list->lock);

	CDListNode* new_node = MemoryPool_AllocateNode(list->pool);

	if (!new_node) {
		mtx_unlock(&list->lock);
		return false; // Memory allocation failed
	}

	new_node->data = data;
	
	if (list->head == NULL) {
		// List is empty
		list->head = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	} else {
		// Insert at the end
		CDListNode* tail = list->head->prev; // 1. 找到当前的尾节点
		new_node->next = list->head; // 2. 新节点的 next 指向头节点
		new_node->prev = tail; // 3. 新节点的 prev 指向旧尾节点
		tail->next = new_node; // 4. 旧尾节点的 next 指向新节点
		list->head->prev = new_node; // 5. 头节点的 prev 指向新节点


		// A->next => A , A->prev => A, tail===A

		// B->prev => A
		// A->next => B
		// A->prev => B


	}

	list->size++;
	mtx_unlock(&list->lock);
	return true;
}

bool List_Prepend(CircularDoublyLinkedList* list, void* data) {
	List_Append(list, data); // 先添加到末尾

	if (list->size > 1) {
		// 如果列表中有多个节点，旋转一次使得新节点成为头节点

		// NOTE： 将新节点设置为头节点的操作放在 Append 中完成
		// List_RotateBackward(list);
		list->head = list->head->prev; // 将头节点指向上一个节点
	}

	return true;
}

void List_DeleteNode(CircularDoublyLinkedList* list, CDListNode* node) {
	if (!list || !node) {
		return;
	}
	mtx_lock(&list->lock);

	// TODO.. 应该有更复杂的逻辑来验证该节点是否在列表中

	if (list->size ==1 || list->head == node) {
		// 删除头节点或列表中只有一个节点的情况
		list->head = NULL; // 清空列表
	} else {
		// 删除非头节点
		node->prev->next = node->next; // 前一个节点的 next 指向下一个节点
		node->next->prev = node->prev; // 下一个节点的 prev 指向前一个节点
		
		if (list->head == node) {
			// 如果删除的是头节点，更新头节点
			list->head = node->next;
		}


		// A <-> B <-> C	node=>B, node->prev=>A, node->next=>C
		// A->next => C
		// C->prev => A

		// A <-> B <-> C  node=>A, node->prev=>C, node->next=>B
		// C->next => B
		// B->prev => C
		// B <=> C



	}

	if (list->user_free_func && node->data) {
		list->user_free_func(node->data);
	}

	MemoryPool_FreeNode(list->pool, node);
	list->size--;
	mtx_unlock(&list->lock);
}

CDListNode* List_Find(CircularDoublyLinkedList* list, const void* data_to_find,
	CompareFunc compare_func) {
	if (!list || !data_to_find || !compare_func) {
		return NULL;
	}
	mtx_lock(&list->lock);

	CDListNode* found_node = NULL;
	CDListNode* current = list->head;

	for (size_t i = 0; i < list->size; i++)
	{
		if (compare_func(current->data, data_to_find) == 0) {
			found_node = current; // 找到匹配的节点
			break;
		}
		current = current->next; // 继续遍历下一个节点
	}


	mtx_unlock(&list->lock);
	return found_node;
}

// 当前的头节点变成尾节点，它原来的下一个节点将成为新的头节点
void List_RotateForward(CircularDoublyLinkedList* list) {
	if (!list || list->size < 2) {
		return; // 无需旋转
	}
	mtx_lock(&list->lock);
	list->head = list->head->next; // 将头节点指向下一个节点
	mtx_unlock(&list->lock);
}

void List_RotateBackward(CircularDoublyLinkedList* list) {
	if (!list || list->size < 2) {
		return; // 无需旋转
	}
	mtx_lock(&list->lock);

	list->head = list->head->prev; // 将头节点指向上一个节点
	mtx_unlock(&list->lock);
}


void List_ForEach(CircularDoublyLinkedList* list, ActionFunc action_func,
	void* context) {

	if (!list || !action_func || !list->head) return;

	mtx_lock(&list->lock);

	CDListNode* current = list->head;

	for (size_t i = 0; i < list->size; i++)
	{
		action_func(current->data, context);
		current = current->next; // 继续遍历下一个节点
	}
	mtx_unlock(&list->lock);
}

void* CDListNode_GetData(const CDListNode* node) {
	return node ? node->data : NULL;
    
}

size_t List_GetSize(const CircularDoublyLinkedList* list) {
	return list ? list->size : 0;
}

CDListNode* List_GetHeadNode(CircularDoublyLinkedList* list) {
	if (!list) {
		return NULL;
	}	

	mtx_lock(&list->lock);
	CDListNode* head_node = list->head;
	mtx_unlock(&list->lock);
	return head_node;
}