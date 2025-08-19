#include <stdio.h>
#include <stdlib.h>
#include "DynamicArray.h"

#define INITIAL_CAPACITY 10

// �ڲ���������������������ʱ�򣬽�������
static int resize_array(DynamicArray *arr, size_t new_capacity)
{

	// realloc arr->data �ϵ�Ƥ�ĵ�ַ
	Data *new_data = realloc(arr->data, new_capacity * sizeof(Data));
	// 1. ��������ԭ�����ݣ���ַû�䣬ԭ�ȵ��ϵ�ַ�Ա��������������ϵĿհ׵�ַ��
	// 2. �ձ������������ݣ���ַ�仯��ԭ�ȵ�����ȫ����Ǩ���µĵ�ַ�����ң����ԭ�Ⱦɵ�ַ������

	if (!new_data)
	{
		// reallocʧ�ܣ��ڴ治��!
		return -1;
		// ������һ���أ���ζ�ţ�ԭ�ȵ��Ǹ�arr->data ���ڣ�
	}

	// realloc�ɹ�֮�󣬲Ż����µĵ�ַȥ���½ṹ��
	arr->data = new_data;
	arr->capacity = new_capacity;

	return 0;
}

// ��������ʼ��һ����̬����
DynamicArray *create_array(size_t initial_capcity)
{

	if (initial_capcity == 0)
	{
		initial_capcity == INITIAL_CAPACITY;
	}
	DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));

	/*
	typedef struct {
	Data* data;		// ָ��洢���ݵ������ڴ��; ָ��һ��������׵�ַ
	// int* data;	// ˵���ˣ����ǿ��Էźܶ�int���͵����ݣ���Ϊָ�����һ���������ϵĵ�ַ�ռ�

	size_t size;

	size_t capacity;
	} DynamicArray;

	*/
	if (!arr)
		return NULL;

	arr->data = (Data *)malloc(initial_capcity * sizeof(Data));
	// arr��Ϊ�ṹ�����������������Ա����������Ա�У�����Ҫ�ľ���Data* data;
	// dataָ��һ���µ��������ϵ��ڴ�ռ�
	// �������ռ䣬���ڱ�����ָ�����һ��capcity = 10������

	if (!arr->data)
	{
		free(arr);
		return NULL;
	}

	arr->size = 0;
	arr->capacity = initial_capcity;

	return arr;
}

// �������飬�ͷ��ڴ�
void destroy_array(DynamicArray *arr)
{
	if (arr)
	{
		free(arr->data);
		free(arr);
	}
}

// ������ĩβ׷��Ԫ�� Amortized O(1)
void array_append(DynamicArray *arr, Data value)
{
	// ����Ƿ�Ҫ����
	if (arr->size >= arr->capacity)
	{
		size_t new_capacity = arr->capacity * 2;
		resize_array(arr, new_capacity);
	}

	arr->data[arr->size] = value;
	arr->size++;
}

// ��ȡָ��������Ԫ��
// ����һ��ָ�룬�Ա��ܹ�����Ƿ�ɹ������������Ч������NULL
Data *array_read(DynamicArray *arr, size_t index)
{
	if (index >= arr->size)
	{
		return NULL;
	}

	// address = base_address + index * sizeof(element);
	return &(arr->data[index]);
}

// ����ָ��������Ԫ��
// ����0��ʾ�ɹ�������-1��ʾʧ��
int array_update(DynamicArray *arr, size_t index, Data value)
{
	if (index >= arr->size)
	{
		return -1;
	}

	arr->data[index] = value;
	return 0;
}

int array_insert(DynamicArray *arr, size_t index, Data value)
{
	if (index > arr->size)
	{
		return -1;
		// ����Խ�磬������ĩβ����, index == size
	}

	// ����Ƿ�Ҫ����
	if (arr->size >= arr->capacity)
	{

		// ������Լ The Function Contract
		if (resize_array(arr, arr->capacity * 2) != 0)
		{
			return -1;
		}
	}

	for (size_t i = arr->size; i > index; --i)
	{
		arr->data[i] = arr->data[i - 1];
	}

	/*

	arr->data = [10, 20, 30, 40]
	arr->data = [10, 20, 30, 40, ��]


	arr->data = [10, 99, 20, 30, 40, ��]
	arr->size = 4;
	index = 1 Ŀ��λ��
	capacity 8

	1.
		i arr->size i = 4;
		i > index 4>1 t
		arr->data[4] = arr->data[3];
		������3��ֵ(40) ���Ƶ� ����4�ϣ��հף�
			arr->data = [10, 20, 30, ��, 40]


	*/

	arr->data[index] = value;

	arr->size++;
	return 0;
}

// ɾ��ָ��������Ԫ��
int array_delete(DynamicArray *arr, size_t index)
{
	if (index >= arr->size)
	{
		return -1;
	}

	for (size_t i = index; i < arr->size - 1; ++i)
	{
		arr->data[i] = arr->data[i + 1];
	}

	/*

	arr->data = [10, 20, 30, 40, 50]

	arr->data = [10, , 30, 40, ��]


	*/

	arr->size--;

	// size capacity 1/4,

	if (arr->size > 0 && arr->size <= arr->capacity / 4 && arr->capacity > INITIAL_CAPACITY)
	{
		size_t new_capacity = arr->capacity / 2;

		// ��֤���ݺ��������Ȼ�ܹ�װ��������Ԫ�أ����Ҳ���С�ڳ�ʼ����
		if (new_capacity < arr->size)
		{
			new_capacity = arr->size;
		}

		if (new_capacity < INITIAL_CAPACITY)
		{
			new_capacity = INITIAL_CAPACITY;
		}

		printf("\n---> [���ݾ���!] Size (%zu) <= Capacity/4 (%zu). ׼�������� %zu. \n",
			   arr->size, arr->capacity / 4, new_capacity);

		resize_array(arr, new_capacity);
	}

	return 0;
}

// print_array�ڶ���������Ҫ�󴫵�һ��ָ������ָ��
// ������ݵĺ����������������ǲ���������const void*
void print_array(const DynamicArray *arr, void (*print_func)(const void *data))
{
	if (!print_func)
	{
		printf("����δ�ṩ��Ч�Ĵ�ӡ����!\n");
		return;
	}

	printf("Array (Size: %zu, Capacity: %zu): [\n", arr->size, arr->capacity);

	for (size_t i = 0; i < arr->size; i++)
	{
		printf("	");

		// �ؼ��ǣ������ⲿ����ĺ���ָ�룬����ӡÿһ��Ԫ��
		// ������Ҫ����ÿ��Ԫ�صĵ�ַ &arr->data[i]
		// ��Ϊprint_func����һ��void*ָ��

		print_func(&arr->data[i]);
		printf("\n");
	}

	printf("]\n");
}