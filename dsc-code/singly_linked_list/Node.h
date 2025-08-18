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
	// 一定得是一个嵌套的结构体
	struct Node* next;
} Node;

Node* createNode(Data data);

// appendNode，当链表为空的时候，Head 是NULL时候，appendnode需要将head指向新创建的节点。
// 这涉及的是head指针本身
void appendNode(Node** headRef, Data data);

void prependNode(Node** headRef, Data data);

void printList(Node* head, void (*print_func)(const void* data));

// 这个函数执行完毕之后，会返回一个指向Node结构体的指针。 Node*
// 泛型
// 第三个参数，意味着，第三个参数传递过来的是一个函数的地址，这个函数必须满足 1. 返回int类型 
// 2. 这个函数必须接受两个const void*类型的参数
// node->data
// 可变参数 printf(%D);
// 调用契约
// 1. 它正在遍历的是当前节点的数据
// 2. 你一开始传递给它的目标数据
// context pointer 上下文指针
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
