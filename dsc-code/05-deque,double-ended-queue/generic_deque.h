#pragma once
#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Declaration ---
typedef struct Deque Deque;

// --- Public API Prototypes ---

/**
 * @brief 创建一个新的泛型双端队列（基于循环数组）。
 *
 * @param capacity 队列的最大容量。
 * @param element_size 每个元素的大小（字节）。
 * @return 成功时返回指向新队列的指针，失败返回 NULL。
 */
Deque* deque_create(size_t capacity, size_t element_size);

/**
 * @brief 销毁双端队列并释放所有内存。
 */
void deque_destroy(Deque** p_deque);

/**
 * @brief 在队头添加一个元素。
 */
bool deque_push_front(Deque* dq, const void* element_data);

/**
 * @brief 在队尾添加一个元素。
 */
bool deque_push_back(Deque* dq, const void* element_data);

/**
 * @brief 从队头移除一个元素。
 */
bool deque_pop_front(Deque* dq, void* output_buffer);

/**
 * @brief 从队尾移除一个元素。
 */
bool deque_pop_back(Deque* dq, void* output_buffer);

/**
 * @brief 查看队头元素。
 */
bool deque_peek_front(const Deque* dq, void* output_buffer);

/**
 * @brief 查看队尾元素。
 */
bool deque_peek_back(const Deque* dq, void* output_buffer);

bool deque_is_empty(const Deque* dq);
bool deque_is_full(const Deque* dq);
size_t deque_get_size(const Deque* dq);