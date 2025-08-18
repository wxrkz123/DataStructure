#define _CRT_SECURE_NO_WARNINGS
#include "DoublyLinkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
* ����һ���µĽڵ㲢��ʼ�������ݡ�
* @note ����һ���ڲ�������ͨ����ֱ�ӵ��á�
* @param songData ����������Ϣ�Ľṹ�塣
* @return ����ָ���´����ڵ��ָ�룬����ڴ����ʧ���򷵻� NULL��
*/
static Node* _createNode(Song songData) {

	// 1.Ϊ�ڵ�����ڴ�
	Node* newNode = (Node*)malloc(sizeof(Node));

	if (!newNode) {
		perror("Failed to allocate memory for new node");
		return NULL; // �ڴ����ʧ��
	}

	// 2.��ʼ���ڵ����ݣ�Ϊ�ڵ������е��ַ�����Ա�����ڴ沢�Ҹ������ݡ�
	// why?
	// ��Ϊ songData �е��ַ�����Ա��ָ���ַ�����������ָ�룬
	// ֱ�Ӹ�ֵ�ᵼ�¶���ڵ�ָ��ͬһ�ڴ��ַ��
	newNode->data.title = (char*)malloc(strlen(songData.title) + 1);
	newNode->data.artist = (char*)malloc(strlen(songData.artist) + 1);

	// ����ΪʲôҪ +1��
	// "Hello" ������ 99 ���ڴ���...
	// ... [H] [e] [l] [l] [o] [\0] [99�Ķ�������] ...
	// ���ַ� '\0' ���ַ����Ľ�����־
	// Null terminator
	// [H] [e] [l] [l] [o] [\0]
	// strlen �����ַ����Ŀɼ��ַ���
	// strlen("Hello")  -> return 5; û�м��� '\0' �ĳ��� 5+1;

	if (!newNode->data.title || !newNode->data.artist) {
		perror("Failed to allocate memory for song data");
		free(newNode->data.title);
		free(newNode->data.artist);
		free(newNode);
		return NULL; // �ڴ����ʧ��
	}

	// strcpy ����ַ�����Դ���Ƶ�Ŀ�꣬���� '\0' �ַ�
	// [H] [e] [l] [l] [o] 
	strcpy(newNode->data.title, songData.title);
	strcpy(newNode->data.artist, songData.artist);
	newNode->data.duration = songData.duration;

	// 3.��ʼ��ָ��
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}


DoublyLinkedList* createList() {
	DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
	if (!list) {
		perror("Failed to allocate memory for the list");
		return NULL; // �ڴ����ʧ��
	}
	// ��ʼ������
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

	// ���������ͷ�ÿ���ڵ�
	// current ָ�������ͷ�ڵ�
	// current ��һ����ʱ��ָ��
	Node* current = list->head;

	while (current != NULL) {
		Node* nextNode = current->next; // ������һ���ڵ��ָ��
		free(current->data.title); // �ͷŸ��������ڴ�
		free(current->data.artist); // �ͷ������������ڴ�
		free(current); // �ͷŵ�ǰ�ڵ��ڴ�
		current = nextNode; // �ƶ�����һ���ڵ�
	}

	free(list); // �ͷ����������������ڴ�
}

bool append(DoublyLinkedList* list, Song songData) {
	if (!list) {
		perror("List is NULL, cannot append.");
		return false; // ����Ϊ��
	}

	Node* newNode = _createNode(songData);
	if (!newNode) {
		return false; // �ڵ㴴��ʧ��
	}

	if (list->head == NULL) {
		// ����Ϊ��ʱ������ͷ�ڵ��β�ڵ�
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		// ����ǿ�ʱ�����½ڵ���ӵ�β��
		list->tail->next = newNode; // ����ǰβ�ڵ�� next ָ���½ڵ�

		// ������ͷ
		// list ��һ��ָ�룬��ָ��һ��DoublyLinkedList�ṹ��
		// ����ṹ����ʲô����������Ĺ����� head, tail, size

		// list->tail ��һ��ָ�룬��ָ�������β�ڵ� 
		// -> һ����ͷ����� ָ���Ա���������

	// list->tail ͨ��listָ�룬�ҵ�����ָ����Ǹ�DoublyLinkedList�ṹ�壬 �������tail��Ա
		// tail ���ó�ԱҲ��һ��ָ�룬���������� Node*��ָ����������һ���ڵ���ڴ��ַ��

		// list->tail->next ��һ��ָ�룬��ָ����������һ���ڵ�� next ��Ա
		// (list->tail)->next;
		// ��ȡ��������(list)��β�ڵ�(tail)�� next ָ��
		newNode->prev = list->tail; // �½ڵ�� prev ָ��ǰβ�ڵ�
		list->tail = newNode; // ����β�ڵ�Ϊ�½ڵ�
	}

	list->size++; // ��������Ĵ�С����
	return true; // �ɹ���ӽڵ�
}

bool prepend(DoublyLinkedList* list, Song songData) {
	if (!list) {
		perror("List is NULL, cannot prepend.");
		return false; // ����Ϊ��
	}

	Node* newNode = _createNode(songData);
	if (!newNode) {
		return false; // �ڵ㴴��ʧ��
	}

	if (list->head == NULL) {
		// ����Ϊ��ʱ������ͷ�ڵ��β�ڵ�
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		// ����ǿ�ʱ�����½ڵ���ӵ�ͷ��
		newNode->next = list->head; // �½ڵ�� next ָ��ǰͷ�ڵ�
		list->head->prev = newNode; // ��ǰͷ�ڵ�� prev ָ���½ڵ�
		list->head = newNode; // ����ͷ�ڵ�Ϊ�½ڵ�
	}

	list->size++;
	return true; // �ɹ���ӽڵ�
}

bool insertAfter(DoublyLinkedList* list, Node* targetNode, Song songData) {
	if (!list || !targetNode) {
		perror("List or target node is NULL, cannot insert.");
		return false; // �����Ŀ��ڵ�Ϊ��
	}
	
	// �����µĽڵ�
	Node* newNode = _createNode(songData);
	// JS


	if (!newNode) {
		return false; // �ڵ㴴��ʧ��
	}


	newNode->next = targetNode->next; // �½ڵ�� next ָ��Ŀ��ڵ����һ���ڵ�
	newNode->prev = targetNode; // �½ڵ�� prev ָ��Ŀ��ڵ�

	if (targetNode->next != NULL) {
		targetNode->next->prev = newNode; // ȷ��������β������
	}
	else {
		list->tail = newNode; // ���Ŀ��ڵ���β�ڵ㣬����β�ڵ�Ϊ�½ڵ�
	}
	targetNode->next = newNode; // Ŀ��ڵ�� next ָ���½ڵ�
	list->size++; // ��������Ĵ�С����
	return true; // �ɹ�����ڵ�
}

// list ָ�������ָ�룬 nodeToDelete��ָ��Ҫɾ���Ľڵ��ָ��
bool deleteNode(DoublyLinkedList* list, Node* nodeToDelete) {
	if (!list || !nodeToDelete) {
		perror("List or node to delete is NULL.");
		return false; // �����ڵ�Ϊ��
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
	free(nodeToDelete); // �ͷŽڵ��ڴ�	

	list->size--; // ��������Ĵ�С����

	return true;
}

Node* findByTitle(const DoublyLinkedList* list, const char* title) {
	if (!list || !title) {
		perror("List or title is NULL, cannot find.");
		return NULL; // ��������Ϊ��
	}

	Node* current = list->head;

	while (current != NULL) {
		if (strcmp(current->data.title, title) == 0) {
			return current; // �ҵ�ƥ��Ľڵ�
		}
		current = current->next; // �ƶ�����һ���ڵ�
	}

	return NULL; // û���ҵ�ƥ��Ľڵ�
}

void printListForward(const DoublyLinkedList* list) {
	if (!list || !list->head) {
		printf("List is empty.\n");
		return; // ����Ϊ��
	}

	printf("---Playlist (Size: %d, Forword) ---\n", list->size);
	Node* current = list->head;
	int index = 1;
	while (current != NULL) {
		printf("%d. Title: %s, Artist: %s, Duration: %d seconds\n",
			index++, current->data.title, current->data.artist, current->data.duration);
		current = current->next; // �ƶ�����һ���ڵ�
	}
	printf("--- End of Playlist ---\n");
}

void printListBackward(const DoublyLinkedList* list) {
	if (!list || !list->tail) {
		printf("List is empty.\n");
		return; // ����Ϊ��
	}

	printf("---Playlist (Size: %d, Forword) ---\n", list->size);
	Node* current = list->tail;
	int index = list->size;
	while (current != NULL) {
		printf("%d. Title: %s, Artist: %s, Duration: %d seconds\n",
			index--, current->data.title, current->data.artist, current->data.duration);
		current = current->prev; // �ƶ�����һ���ڵ�
	}
	printf("--- End of Playlist ---\n");
}