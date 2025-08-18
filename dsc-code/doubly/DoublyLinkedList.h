#pragma once
#include <stdbool.h>

// 模拟节点存储数据
typedef struct {
	// char title[100]; const 一旦定义，就不能更改，一直都是100
	// 这里使用动态内存分配来存储字符串
	// 指向一个指针
	// 这样可以在运行时动态分配内存
	char* title;
	char* artist;
	int duration; // 单位为秒
} Song;


// 双向链表
typedef struct Node {
	Song data;
	struct Node* next;
	struct Node* prev;
} Node;


//  定义一个管理器
typedef struct {
	Node* head;
	Node* tail;
	int size;
} DoublyLinkedList;

DoublyLinkedList* createList();

void freeList(DoublyLinkedList* list);

bool append(DoublyLinkedList* list, Song songData);

bool prepend(DoublyLinkedList* list, Song songData);

bool insertAfter(DoublyLinkedList* list, Node* targetNode, Song songData);

// list 指向链表的指针， nodeToDelete是指向要删除的节点的指针
bool deleteNode(DoublyLinkedList* list, Node* nodeToDelete);

Node* findByTitle(const DoublyLinkedList* list, const char* title);

void printListForward(const DoublyLinkedList* list);

void printListBackward(const DoublyLinkedList* list);