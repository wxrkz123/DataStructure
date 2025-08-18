#define _CRT_SECURE_NO_WARNINGS
#include "DoublyLinkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
* 创建一个新的节点并初始化其数据。
* @note 这是一个内部函数，通常不直接调用。
* @param songData 包含歌曲信息的结构体。
* @return 返回指向新创建节点的指针，如果内存分配失败则返回 NULL。
*/
static Node* _createNode(Song songData) {

	// 1.为节点分配内存
	Node* newNode = (Node*)malloc(sizeof(Node));

	if (!newNode) {
		perror("Failed to allocate memory for new node");
		return NULL; // 内存分配失败
	}

	// 2.初始化节点数据，为节点数据中的字符串成员分配内存并且复制内容。
	// why?
	// 因为 songData 中的字符串成员是指向字符串字面量的指针，
	// 直接赋值会导致多个节点指向同一内存地址，
	newNode->data.title = (char*)malloc(strlen(songData.title) + 1);
	newNode->data.artist = (char*)malloc(strlen(songData.artist) + 1);

	// 讲解为什么要 +1？
	// "Hello" 和整数 99 在内存中...
	// ... [H] [e] [l] [l] [o] [\0] [99的二进制数] ...
	// 空字符 '\0' 是字符串的结束标志
	// Null terminator
	// [H] [e] [l] [l] [o] [\0]
	// strlen 计算字符串的可见字符数
	// strlen("Hello")  -> return 5; 没有计算 '\0' 的长度 5+1;

	if (!newNode->data.title || !newNode->data.artist) {
		perror("Failed to allocate memory for song data");
		free(newNode->data.title);
		free(newNode->data.artist);
		free(newNode);
		return NULL; // 内存分配失败
	}

	// strcpy 会把字符串从源复制到目标，包括 '\0' 字符
	// [H] [e] [l] [l] [o] 
	strcpy(newNode->data.title, songData.title);
	strcpy(newNode->data.artist, songData.artist);
	newNode->data.duration = songData.duration;

	// 3.初始化指针
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}


DoublyLinkedList* createList() {
	DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
	if (!list) {
		perror("Failed to allocate memory for the list");
		return NULL; // 内存分配失败
	}
	// 初始化链表
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

void freeList(DoublyLinkedList* list) {
	if (!list)
	{
		perror("List is NULL, nothing to free.\n");
		return;
	}

	// 遍历链表并释放每个节点
	// current 指向链表的头节点
	// current 是一个临时的指针
	Node* current = list->head;

	while (current != NULL) {
		Node* nextNode = current->next; // 保存下一个节点的指针
		free(current->data.title); // 释放歌曲标题内存
		free(current->data.artist); // 释放艺术家名称内存
		free(current); // 释放当前节点内存
		current = nextNode; // 移动到下一个节点
	}

	free(list); // 释放链表管理器本身的内存
}

bool append(DoublyLinkedList* list, Song songData) {
	if (!list) {
		perror("List is NULL, cannot append.");
		return false; // 链表为空
	}

	Node* newNode = _createNode(songData);
	if (!newNode) {
		return false; // 节点创建失败
	}

	if (list->head == NULL) {
		// 链表为空时，设置头节点和尾节点
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		// 链表非空时，将新节点添加到尾部
		list->tail->next = newNode; // 将当前尾节点的 next 指向新节点

		// 两个箭头
		// list 是一个指针，它指向一个DoublyLinkedList结构体
		// 这个结构体是什么，整个链表的管理器 head, tail, size

		// list->tail 是一个指针，它指向链表的尾节点 
		// -> 一个箭头运算符 指针成员访问运算符

	// list->tail 通过list指针，找到它所指向的那个DoublyLinkedList结构体， 获得它的tail成员
		// tail ，该成员也是一个指针，它的类型是 Node*，指向链表的最后一个节点的内存地址。

		// list->tail->next 是一个指针，它指向链表的最后一个节点的 next 成员
		// (list->tail)->next;
		// 获取整个链表(list)的尾节点(tail)的 next 指针
		newNode->prev = list->tail; // 新节点的 prev 指向当前尾节点
		list->tail = newNode; // 更新尾节点为新节点
	}

	list->size++; // 增加链表的大小计数
	return true; // 成功添加节点
}

bool prepend(DoublyLinkedList* list, Song songData) {
	if (!list) {
		perror("List is NULL, cannot prepend.");
		return false; // 链表为空
	}

	Node* newNode = _createNode(songData);
	if (!newNode) {
		return false; // 节点创建失败
	}

	if (list->head == NULL) {
		// 链表为空时，设置头节点和尾节点
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		// 链表非空时，将新节点添加到头部
		newNode->next = list->head; // 新节点的 next 指向当前头节点
		list->head->prev = newNode; // 当前头节点的 prev 指向新节点
		list->head = newNode; // 更新头节点为新节点
	}

	list->size++;
	return true; // 成功添加节点
}

bool insertAfter(DoublyLinkedList* list, Node* targetNode, Song songData) {
	if (!list || !targetNode) {
		perror("List or target node is NULL, cannot insert.");
		return false; // 链表或目标节点为空
	}
	
	// 创建新的节点
	Node* newNode = _createNode(songData);
	// JS


	if (!newNode) {
		return false; // 节点创建失败
	}


	newNode->next = targetNode->next; // 新节点的 next 指向目标节点的下一个节点
	newNode->prev = targetNode; // 新节点的 prev 指向目标节点

	if (targetNode->next != NULL) {
		targetNode->next->prev = newNode; // 确保不是在尾部插入
	}
	else {
		list->tail = newNode; // 如果目标节点是尾节点，更新尾节点为新节点
	}
	targetNode->next = newNode; // 目标节点的 next 指向新节点
	list->size++; // 增加链表的大小计数
	return true; // 成功插入节点
}

// list 指向链表的指针， nodeToDelete是指向要删除的节点的指针
bool deleteNode(DoublyLinkedList* list, Node* nodeToDelete) {
	if (!list || !nodeToDelete) {
		perror("List or node to delete is NULL.");
		return false; // 链表或节点为空
	}

	if (nodeToDelete->prev != NULL) {
		nodeToDelete->prev->next = nodeToDelete->next;
	}
	else {
		list->head = nodeToDelete->next;
	}

	if (nodeToDelete->next != NULL) {
		nodeToDelete->next->prev = nodeToDelete->prev;
	}
	else {
		list->tail = nodeToDelete->prev;
	}

	free(nodeToDelete->data.title);
	free(nodeToDelete->data.artist);
	free(nodeToDelete); // 释放节点内存	

	list->size--; // 减少链表的大小计数

	return true;
}

Node* findByTitle(const DoublyLinkedList* list, const char* title) {
	if (!list || !title) {
		perror("List or title is NULL, cannot find.");
		return NULL; // 链表或标题为空
	}

	Node* current = list->head;

	while (current != NULL) {
		if (strcmp(current->data.title, title) == 0) {
			return current; // 找到匹配的节点
		}
		current = current->next; // 移动到下一个节点
	}

	return NULL; // 没有找到匹配的节点
}

void printListForward(const DoublyLinkedList* list) {
	if (!list || !list->head) {
		printf("List is empty.\n");
		return; // 链表为空
	}

	printf("---Playlist (Size: %d, Forword) ---\n", list->size);
	Node* current = list->head;
	int index = 1;
	while (current != NULL) {
		printf("%d. Title: %s, Artist: %s, Duration: %d seconds\n",
			index++, current->data.title, current->data.artist, current->data.duration);
		current = current->next; // 移动到下一个节点
	}
	printf("--- End of Playlist ---\n");
}

void printListBackward(const DoublyLinkedList* list) {
	if (!list || !list->tail) {
		printf("List is empty.\n");
		return; // 链表为空
	}

	printf("---Playlist (Size: %d, Forword) ---\n", list->size);
	Node* current = list->tail;
	int index = list->size;
	while (current != NULL) {
		printf("%d. Title: %s, Artist: %s, Duration: %d seconds\n",
			index--, current->data.title, current->data.artist, current->data.duration);
		current = current->prev; // 移动到下一个节点
	}
	printf("--- End of Playlist ---\n");
}