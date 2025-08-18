#pragma once
#include <stdbool.h>
#include <stddef.h>

// --- Opaque Pointer Type ---
// �û�ֻ�ܿ�����Щ���͵�ָ�룬����ֱ�ӷ������ڲ��ṹ ���� OOP��װ

typedef struct DoublyLinkedList DoublyLinkedList;
typedef struct DListNode DListNode;

// --- Callback Function Pointer Typedefs ---

// �����û���Ҫ�ṩ�Ļص��������ͣ���ǿ����ɶ���

typedef int (*CompareFunc)(const void* data1, const void* data2);
typedef void (*PrintFunc)(const void* data);
typedef void (*FreeFunc)(void* data);
typedef void (*ActionFunc)(void* data, void* context);

// MemoryPool ��غ���ָ������
// �ڴ�ؽṹ����Ԥ�ȷ���һ���ڴ棬���ڴ洢����ڵ㣬����Ƶ�����ڴ������ͷŲ�����
// ���ҽ��仮��Ϊ����̶���С�Ŀ飬�Ա���ٷ�����ͷš��ڲ�ͨ��һ������������������Щ�ڴ�顣



// API FUNCTION DECLARATIONS

/**
 * @brief ����һ���µ�˫������
 * 
 * @param initial_capacity �ڴ�س�ʼ������������Ԥ����ڵ���ڲ��ṹ��
 * @param free_func �û��Զ�����ͷ����ݵĻص��������������ٻ�ڵ�ɾ��ʱ����
 * @return DoublyLinkedList* �´���������ָ�룬ʧ��ʱ����NULL
 * 
 * @note �������������Ӧ�����ٺ����ͷ�������Դ
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