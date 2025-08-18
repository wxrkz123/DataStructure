#pragma once
#include <stddef.h>

typedef struct {
	int id;
	char name[50];
	int age;
} Student;


typedef Student Data;


typedef struct {
	Data data;
	// һ������һ��Ƕ�׵Ľṹ��
	struct Node* next;
} Node;

Node* createNode(Data data);

// appendNode��������Ϊ�յ�ʱ��Head ��NULLʱ��appendnode��Ҫ��headָ���´����Ľڵ㡣
// ���漰����headָ�뱾��
void appendNode(Node** headRef, Data data);

void prependNode(Node** headRef, Data data);

void printList(Node* head, void (*print_func)(const void* data));

// �������ִ�����֮�󣬻᷵��һ��ָ��Node�ṹ���ָ�롣 Node*
// ����
// ��������������ζ�ţ��������������ݹ�������һ�������ĵ�ַ����������������� 1. ����int���� 
// 2. ������������������const void*���͵Ĳ���
// node->data
// �ɱ���� printf(%D);
// ������Լ
// 1. �����ڱ������ǵ�ǰ�ڵ������
// 2. ��һ��ʼ���ݸ�����Ŀ������
// context pointer ������ָ��
Node* findNode(
	Node* head,
	const void* target_data,
	int (*compare_func) (const void* a, const void* b, void* context),
	void* context
);

void deleteNode(
	Node** headRef,
	const void* target_data,
	int (*compare_func) (const void* a, const void* b, void* context),
	void* context
);

void updateNode(
	Node** headRef,
	const void* target_data,
	Data newData,
	int (*compare_func) (const void* a, const void* b, void* context),
	void* context
);

void freeList(Node** headRef, void (*free_data_func)(void* data));
