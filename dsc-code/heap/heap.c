#include "heap.h"
#include <stdio.h>
#include <string.h> // For memcpy

// ========== �ڲ��������� (�����Ⱪ¶) ==========

// ��������Item���͵�ֵ
static void swap(Item* a, Item* b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// "�ϸ�"��������ָ�������Ľڵ����ϵ�������ά����������
static void heapify_up(Heap* h, size_t index) {
    if (index == 0) return; // ���ڵ��޷��ϸ�

    size_t parent_index = (index - 1) / 2;

	// ������������Ϊindex�Ľڵ㣺
	// �������ӽڵ�����Ϊ 2 * index + 1
	// �������ӽڵ�����Ϊ 2 * index + 2
	// �����ǰ�ڵ������ӽڵ㣬���丸�ڵ�����Ϊ (index - 1) / 2
	// �����ǰ�ڵ������ӽڵ㣬���丸�ڵ�����Ϊ (index - 2) / 2
	// C���Ե������������Զ�����ȡ�����������ǿ���ֱ��ʹ�� (index - 1) / 2 �����㸸�ڵ�������
	// parent_index = 3
	// left child_index = 2 * parent_index + 1; // ���ӽڵ����� 7
	//  => parent_index = (index - 1) / 2; // ���ڵ����� 3
	// right child_index = 2 * parent_index + 2; // ���ӽڵ����� 8
	//  => parent_index = (index - 1) / 2; // ���ڵ����� 3

	// 100  90  80  70  60  50  40 
	// 0    1    2   3   4   5   6


    // �����ǰ�ڵ�ȸ��ڵ���򽻻����������ϵ���
    if (h->data[index] > h->data[parent_index]) {
        swap(&h->data[index], &h->data[parent_index]);
        heapify_up(h, parent_index);
    }
}

// "�³�"��������ָ�������Ľڵ����µ�������ά����������
static void heapify_down(Heap* h, size_t index) {
    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;
    size_t largest_index = index;

    // �ҳ���ǰ�ڵ㡢���ӽڵ㡢���ӽڵ��е����ֵ����
    if (left_child_index < h->size && h->data[left_child_index] > h->data[largest_index]) {
        largest_index = left_child_index;
    }
    if (right_child_index < h->size && h->data[right_child_index] > h->data[largest_index]) {
        largest_index = right_child_index;
    }

    // ������ֵ���ǵ�ǰ�ڵ㣬�򽻻����������µ���
    if (largest_index != index) {
        swap(&h->data[index], &h->data[largest_index]);
        heapify_down(h, largest_index);
    }
}

// ��̬���ݶѵ��ڲ��洢
static int heap_resize(Heap* h) {
    size_t new_capacity = h->capacity * 2;
    Item* new_data = (Item*)realloc(h->data, new_capacity * sizeof(Item));
    if (!new_data) {
        perror("Failed to resize heap");
        return -1;
    }
    h->data = new_data;
    h->capacity = new_capacity;
    printf("[DEBUG] Heap resized to capacity %zu\n", new_capacity);
    return 0;
}


// ========== ADT �ӿں���ʵ�� ==========

Heap* heap_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 8; // Ĭ��һ����С�ĳ�ʼ����
    }

    Heap* h = (Heap*)malloc(sizeof(Heap));
    if (!h) {
        perror("Failed to allocate memory for heap structure");
        return NULL;
    }

    h->data = (Item*)malloc(initial_capacity * sizeof(Item));
    if (!h->data) {
        perror("Failed to allocate memory for heap data");
        free(h);
        return NULL;
    }

    h->size = 0;
    h->capacity = initial_capacity;

    return h;
}

void heap_destroy(Heap** h) {
    if (h && *h) {
        free((*h)->data); // �ͷ���������
        (*h)->data = NULL;
        free(*h);         // �ͷŶѽṹ��
        *h = NULL;        // ���ⲿָ����ΪNULL����ֹҰָ��
    }
}

int heap_insert(Heap* h, Item value) {
    if (!h) return -1;

    // ��������ˣ���������
    if (h->size == h->capacity) {
        if (heap_resize(h) != 0) {
            return -1; // ����ʧ��
        }
    }

    // 1. ����Ԫ����ӵ�����ĩβ
    h->data[h->size] = value;
    // 2. ����Ԫ��ִ�� "�ϸ�" ����
    heapify_up(h, h->size);
    // 3. ���ӶѵĴ�С
    h->size++;

    return 0;
}

int heap_extract_max(Heap* h, Item* p_max_value) {
    if (!h || is_heap_empty(h)) {
        return -1; // ��Ϊ�ջ���Ч
    }

    // 1. �Ѷ�Ԫ�ؼ�Ϊ���ֵ
    *p_max_value = h->data[0];

    // 2. �����һ��Ԫ���Ƶ��Ѷ�
    h->data[0] = h->data[h->size - 1];
    h->size--;

    // 3. ���µĶѶ�Ԫ��ִ�� "�³�" ����
    if (h->size > 0) {
        heapify_down(h, 0);
    }

    return 0;
}

int heap_peek(const Heap* h, Item* p_peek_value) {
    if (!h || is_heap_empty(h)) {
        return -1;
    }
    *p_peek_value = h->data[0];
    return 0;
}

int is_heap_empty(const Heap* h) {
    return (h == NULL || h->size == 0);
}

size_t heap_size(const Heap* h) {
    if (!h) return 0;
    return h->size;
}

void heap_print_debug(const Heap* h) {
    if (!h) {
        printf("Heap is NULL.\n");
        return;
    }
    if (is_heap_empty(h)) {
        printf("Heap is empty.\n");
        return;
    }

    printf("Heap (size=%zu, capacity=%zu): [ ", h->size, h->capacity);
    for (size_t i = 0; i < h->size; ++i) {
        printf("%d ", h->data[i]);
    }
    printf("]\n");
}