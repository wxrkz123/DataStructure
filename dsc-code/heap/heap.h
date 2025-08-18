#pragma once
#include <stdlib.h> // for size_t

// 为了代码的可读性和可维护性，定义一个 Item 类型
// 如果需要存储其他类型数据，只需修改这里
typedef int Item;

// 堆结构体定义
typedef struct {
    Item* data;         // 指向存储堆元素的动态数组
    size_t size;        // 堆中当前的元素数量
    size_t capacity;    // 堆的当前容量
} Heap;

/**
 * @brief 创建一个指定初始容量的最大堆
 * @param initial_capacity 堆的初始容量
 * @return 成功则返回指向堆的指针，失败则返回 NULL
 */
Heap* heap_create(size_t initial_capacity);

/**
 * @brief 销毁一个堆，释放所有相关内存
 * @param h 指向要销毁的堆的指针的指针
 */
void heap_destroy(Heap** h);

/**
 * @brief 向堆中插入一个新元素
 * @param h 指向堆的指针
 * @param value 要插入的值
 * @return 成功返回 0，失败（如内存分配失败）返回 -1
 */
int heap_insert(Heap* h, Item value);

/**
 * @brief 从堆中提取（并移除）最大值
 * @param h 指向堆的指针
 * @param p_max_value 指向用于存储最大值的变量的指针
 * @return 成功提取返回 0，如果堆为空则返回 -1
 */
int heap_extract_max(Heap* h, Item* p_max_value);

/**
 * @brief 查看堆顶的最大值（不移除）
 * @param h 指向堆的指针
 * @param p_peek_value 指向用于存储最大值的变量的指针
 * @return 成功查看返回 0，如果堆为空则返回 -1
 */
int heap_peek(const Heap* h, Item* p_peek_value);

/**
 * @brief 检查堆是否为空
 * @param h 指向堆的指针
 * @return 如果堆为空返回 1，否则返回 0
 */
int is_heap_empty(const Heap* h);

/**
 * @brief 获取堆中元素的数量
 * @param h 指向堆的指针
 * @return 堆中元素的数量
 */
size_t heap_size(const Heap* h);

/**
 * @brief (可选) 打印堆的内部数组表示，主要用于调试
 * @param h 指向堆的指针
 */
void heap_print_debug(const Heap* h);