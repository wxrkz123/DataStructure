#pragma once

#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool

// --- Public Enumeration and Opaque Pointer ---

// 使用枚举来清晰地标识要操作的栈
typedef enum {
    STACK_ONE = 1, // 栈1，从数组头部开始
    STACK_TWO = 2  // 栈2，从数组尾部开始
} StackNumber;

// 不透明指针，用户无法看到内部结构
typedef struct SharedStack Stack;

// --- Public API Prototypes ---

/**
 * @brief 创建一个新的共享栈。
 *
 * @param total_capacity 两个栈共享的总容量（元素个数）。
 * @param element_size   每个元素的大小（以字节为单位）。
 * @return 成功时返回指向新栈的指针，失败返回 NULL。
 */
Stack* stack_create(size_t total_capacity, size_t element_size);

/**
 * @brief 销毁一个共享栈并释放所有内存。
 *
 * @param p_stack 指向栈指针的指针，函数会将其置为 NULL。
 */
void stack_destroy(Stack** p_stack);

/**
 * @brief 将一个元素压入指定的栈。
 *
 * @param stack 指向要操作的栈。
 * @param num   要操作的栈编号 (STACK_ONE 或 STACK_TWO)。
 * @param element_data 指向要压入的元素数据的指针。
 * @return 成功返回 true，如果栈满或参数无效则返回 false。
 */
bool stack_push(Stack* stack, StackNumber num, const void* element_data);

/**
 * @brief 从指定的栈弹出一个元素。
 *
 * @param stack 指向要操作的栈。
 * @param num   要操作的栈编号 (STACK_ONE 或 STACK_TWO)。
 * @param output_buffer 指向用于接收弹出元素数据的缓冲区。
 * @return 成功返回 true，如果栈空或参数无效则返回 false。
 */
bool stack_pop(Stack* stack, StackNumber num, void* output_buffer);

/**
 * @brief 查看指定栈的栈顶元素，但不弹出。
 *
 * @param stack 指向要操作的栈。
 * @param num   要操作的栈编号 (STACK_ONE 或 STACK_TWO)。
 * @param output_buffer 指向用于接收栈顶元素数据的缓冲区。
 * @return 成功返回 true，如果栈空或参数无效则返回 false。
 */
bool stack_peek(const Stack* stack, StackNumber num, void* output_buffer);

/**
 * @brief 检查共享空间是否已满。
 *
 * @param stack 指向要检查的栈。
 * @return 如果已满返回 true，否则返回 false。
 */
bool stack_is_full(const Stack* stack);

/**
 * @brief 检查指定的栈是否为空。
 *
 * @param stack 指向要检查的栈。
 * @param num   要检查的栈编号 (STACK_ONE 或 STACK_TWO)。
 * @return 如果该栈为空返回 true，否则返回 false。
 */
bool stack_is_empty(const Stack* stack, StackNumber num);

/**
 * @brief 获取指定栈的当前元素数量。
 *
 * @param stack 指向要操作的栈。
 * @param num   要获取大小的栈编号 (STACK_ONE 或 STACK_TWO)。
 * @return 返回该栈的元素数量。
 */
size_t stack_get_size(const Stack* stack, StackNumber num);

/**
 * @brief 获取共享栈的总容量。
 *
 * @param stack 指向要操作的栈。
 * @return 返回总容量。
 */
size_t stack_get_total_capacity(const Stack* stack);
