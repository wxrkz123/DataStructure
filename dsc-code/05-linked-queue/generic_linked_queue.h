#pragma once

#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Declaration ---
// 用户只知道有 Queue 这个类型，不知道其内部是链表还是数组。
typedef struct LinkedQueue Queue;

// --- Public API Prototypes ---

/**
 * @brief 创建一个新的泛型链式队列。
 *
 * @param element_size 每个元素的大小（以字节为单位）。
 * @return 成功时返回指向新队列的指针，失败返回 NULL。
 */
Queue* queue_create(size_t element_size);

/**
 * @brief 销毁一个队列并释放其所有节点和相关内存。
 *
 * @param p_queue 指向队列指针的指针，函数会将其置为 NULL。
 */
void queue_destroy(Queue** p_queue);

/**
 * @brief 将一个元素添加到队尾（入队）。
 *
 * @param queue 指向要操作的队列。
 * @param element_data 指向要入队的元素数据的指针。
 * @return 成功返回 true，如果内存分配失败或参数无效则返回 false。
 */
bool queue_enqueue(Queue* queue, const void* element_data);

/**
 * @brief 从队头移除一个元素（出队）。
 *
 * @param queue 指向要操作的队列。
 * @param output_buffer 指向用于接收出队元素数据的缓冲区。
 * @return 成功返回 true，如果队列为空或参数无效则返回 false。
 */
bool queue_dequeue(Queue* queue, void* output_buffer);

/**
 * @brief 查看队头元素，但不将其移除。
 *
 * @param queue 指向要操作的队列。
 * @param output_buffer 指向用于接收队头元素数据的缓冲区。
 * @return 成功返回 true，如果队列为空或参数无效则返回 false。
 */
bool queue_peek(const Queue* queue, void* output_buffer);

/**
 * @brief 检查队列是否为空。
 *
 * @param queue 指向要检查的队列。
 * @return 如果队列为空返回 true，否则返回 false。
 */
bool queue_is_empty(const Queue* queue);

/**
 * @brief 获取队列中当前的元素数量。
 *
 * @param queue 指向要操作的队列。
 * @return 返回队列中的元素数量。
 */
size_t queue_get_size(const Queue* queue);