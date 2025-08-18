#pragma once
#include <stdbool.h>

// ģ��ڵ�洢����
typedef struct {
	// char title[100]; const һ�����壬�Ͳ��ܸ��ģ�һֱ����100
	// ����ʹ�ö�̬�ڴ�������洢�ַ���
	// ָ��һ��ָ��
	// ��������������ʱ��̬�����ڴ�
	char* title;
	char* artist;
	int duration; // ��λΪ��
} Song;


// ˫������
typedef struct Node {
	Song data;
	struct Node* next;
	struct Node* prev;
} Node;


//  ����һ��������
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

// list ָ�������ָ�룬 nodeToDelete��ָ��Ҫɾ���Ľڵ��ָ��
bool deleteNode(DoublyLinkedList* list, Node* nodeToDelete);

Node* findByTitle(const DoublyLinkedList* list, const char* title);

void printListForward(const DoublyLinkedList* list);

void printListBackward(const DoublyLinkedList* list);