#pragma once
#include <stddef.h>

typedef struct {
	int id;
	char name[50];
} Student;


typedef Student Data;

// typedef�Ǹ�һ���Ѿ����ڵ�����������һ���µ����֣�������

// �������Ƕ�̬����Ľṹ��
// 

//[{stu1.id =1, stu1.name='Frank'}, {},....]


typedef struct {
	Data* data;		// ָ��洢���ݵ������ڴ��; ָ��һ��������׵�ַ
	// int* data;	// ˵���ˣ����ǿ��Էźܶ�int���͵����ݣ���Ϊָ�����һ���������ϵĵ�ַ�ռ�

	size_t size;

	size_t capacity;
} DynamicArray;

// �����ӿں�������

// ��������ʼ��һ����̬����
DynamicArray* create_array(size_t initial_capcity);

// �������飬�ͷ��ڴ�
void destroy_array(DynamicArray* arr);

// ������ĩβ׷��Ԫ�� Amortized O(1)
void array_append(DynamicArray* arr, Data value);

// ��ȡָ��������Ԫ��
// ����һ��ָ�룬�Ա��ܹ�����Ƿ�ɹ������������Ч������NULL
Data* array_read(DynamicArray* arr, size_t index);

// ����ָ��������Ԫ��
// ����0��ʾ�ɹ�������-1��ʾʧ��
int array_update(DynamicArray* arr, size_t index, Data value);

int array_insert(DynamicArray* arr, size_t index, Data value);

// ɾ��ָ��������Ԫ��
int array_delete(DynamicArray* arr, size_t index);

void print_array(const DynamicArray* arr, void (*print_func)(const void* data));

