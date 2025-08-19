#define _CRT_SECURE_NO_WARNINGS
#include "Node.h"
#include <stdio.h>
#include <stdlib.h>


Node* createNode(Data data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		perror("创建节点失败：内存分配错误");
		return NULL;
	}
	
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}


void appendNode(Node** headRef, Data data) {
	Node* newNode = createNode(data);

	if (newNode == NULL) return;

	if (*headRef == NULL) {
		*headRef = newNode;
		return;
	}

	// head -> [10 | next] ---> [20 | next] --->  newNode --> [30 | next] ---> NULL
	//				head
	//											last newNode

	// newNode --> [30 | next]

	Node* last = *headRef;
	//  Node* last = head;
	while (last->next != NULL) {
		last = last->next;
	}

	last->next = newNode;

}

void prependNode(Node** headRef, Data data) {
	Node* newNode = createNode(data);

	if (newNode == NULL) return;
	newNode->next = *headRef;
	*headRef = newNode;

	// head -> [10 | next] ---> [20 | next] --->  newNode --> [30 | next] ---> NULL
	//				head
	//											last newNode

	// newNode --> [5 | next]
}

void printList(Node* head, void (*print_func)(const void* data)) {
	if (print_func == NULL) {
		printf("错误: 未提供打印函数.\n");
		return;
	}

	printf("链表内容: ");
	Node* current = head;
	while (current != NULL) {
		print_func(&(current->data));
		printf(" -> ");
		current = current->next;
	}
	printf("NULL\n");
}

Node* findNode(
	Node* head,
	const void* target_data,
	int (*compare_func) (const void* a, const void* b, void* context),
	void* context
) {
	if (compare_func == NULL) {
		printf("错误: 未提供比较函数.\n");
		return NULL;
	}

	Node* current = head;
	while (current != NULL) {
		// 调用回调
		if (compare_func(&(current->data), target_data, context) == 0) {
			return current;
		}
		current = current->next;
	}

	return NULL;
}

void deleteNode(
	Node** headRef,
	const void* target_data,
	int (*compare_func) (const void* a, const void* b, void* context),
	void* context
) {
	if (headRef == NULL || *headRef == NULL || compare_func == NULL) {
		printf("错误: 无效的参数.\n");
		return;
	}

	Node* temp = *headRef;
	Node* prev = NULL;

	// 检查头节点是否是目标
	if (compare_func(&(temp->data), target_data, context) == 0) {
		*headRef = temp->next;
		free(temp);
		printf("信息：头节点已经删除！\n");
		return;
	
	}

	while (temp != NULL && compare_func(&temp->data, target_data, context) != 0) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("警告：未找到目标节点，无法删除!\n");
		return;
	}

	// 检查 prev 是否为 NULL  
	if (prev != NULL) {
		prev->next = temp->next;
	}

	free(temp);
	printf("信息：节点已经删除!\n");
	
}

void updateNode(Node** headRef, const void* target_data, Data newData, int(*compare_func)(const void* a, const void* b, void* context), void* context)
{
	Node* nodeToUpdate = findNode(headRef, target_data, compare_func, context);
	if (nodeToUpdate != NULL) {
		nodeToUpdate->data = newData;
		printf("信息：节点已经成功更新!\n");
	}
	else {
		printf("警告: 未找到目标节点，无法更新！\n");
	}
}

//void updateNode(
//	Node* headRef,
//	const void* target_data,
//	Data newData,
//	int (*compare_func) (const void* a, const void* b, void* context),
//	void* context
//) {
//	
//}

void freeList(Node** headRef, void (*free_data_func)(void* data)) {
	if (headRef == NULL) return;

	Node* current = *headRef;
	Node* nextNode;

	while (current != NULL) {
		nextNode = current->next;
		if (free_data_func != NULL) {
			free_data_func(&current->data);
		}
		free(current);
		current = nextNode;
	}

	*headRef = NULL;
}
