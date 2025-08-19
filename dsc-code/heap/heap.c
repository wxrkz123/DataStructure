#include "heap.h"
#include <stdio.h>
#include <string.h> // For memcpy

// ========== 内部辅助函数 (不对外暴露) ==========

// 交换两个Item类型的值
static void swap(Item* a, Item* b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// "上浮"操作：将指定索引的节点向上调整，以维持最大堆性质
static void heapify_up(Heap* h, size_t index) {
    if (index == 0) return; // 根节点无法上浮

    size_t parent_index = (index - 1) / 2;

	// 对于任意索引为index的节点：
	// 它的左子节点索引为 2 * index + 1
	// 它的右子节点索引为 2 * index + 2
	// 如果当前节点是左子节点，则其父节点索引为 (index - 1) / 2
	// 如果当前节点是右子节点，则其父节点索引为 (index - 2) / 2
	// C语言的整数除法会自动向下取整，所以我们可以直接使用 (index - 1) / 2 来计算父节点索引。
	// parent_index = 3
	// left child_index = 2 * parent_index + 1; // 左子节点索引 7
	//  => parent_index = (index - 1) / 2; // 父节点索引 3
	// right child_index = 2 * parent_index + 2; // 右子节点索引 8
	//  => parent_index = (index - 1) / 2; // 父节点索引 3

	// 100  90  80  70  60  50  40 
	// 0    1    2   3   4   5   6


    // 如果当前节点比父节点大，则交换并继续向上调整
    if (h->data[index] > h->data[parent_index]) {
        swap(&h->data[index], &h->data[parent_index]);
        heapify_up(h, parent_index);
    }
}

// "下沉"操作：将指定索引的节点向下调整，以维持最大堆性质
static void heapify_down(Heap* h, size_t index) {
    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;
    size_t largest_index = index;

    // 找出当前节点、左子节点、右子节点中的最大值索引
    if (left_child_index < h->size && h->data[left_child_index] > h->data[largest_index]) {
        largest_index = left_child_index;
    }
    if (right_child_index < h->size && h->data[right_child_index] > h->data[largest_index]) {
        largest_index = right_child_index;
    }

    // 如果最大值不是当前节点，则交换并继续向下调整
    if (largest_index != index) {
        swap(&h->data[index], &h->data[largest_index]);
        heapify_down(h, largest_index);
    }
}

// 动态扩容堆的内部存储
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


// ========== ADT 接口函数实现 ==========

Heap* heap_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 8; // 默认一个较小的初始容量
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
        free((*h)->data); // 释放数据数组
        (*h)->data = NULL;
        free(*h);         // 释放堆结构体
        *h = NULL;        // 将外部指针置为NULL，防止野指针
    }
}

int heap_insert(Heap* h, Item value) {
    if (!h) return -1;

    // 如果堆满了，进行扩容
    if (h->size == h->capacity) {
        if (heap_resize(h) != 0) {
            return -1; // 扩容失败
        }
    }

    // 1. 将新元素添加到数组末尾
    h->data[h->size] = value;
    // 2. 对新元素执行 "上浮" 操作
    heapify_up(h, h->size);
    // 3. 增加堆的大小
    h->size++;

    return 0;
}

int heap_extract_max(Heap* h, Item* p_max_value) {
    if (!h || is_heap_empty(h)) {
        return -1; // 堆为空或无效
    }

    // 1. 堆顶元素即为最大值
    *p_max_value = h->data[0];

    // 2. 将最后一个元素移到堆顶
    h->data[0] = h->data[h->size - 1];
    h->size--;

    // 3. 对新的堆顶元素执行 "下沉" 操作
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