#pragma once
#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool (C99/C11 standard)

// --- Opaque Pointer Declaration ---
// 用户只知道有 Stack 这个类型，但不知道其内部是链表还是数组。
typedef struct LinkedStack Stack;

// --- Public API Prototypes ---

/**
 * @brief 创建一个新的通用链式栈。
 *
 * @param element_size 栈中每个元素的大小（以字节为单位），例如 sizeof(int)。
 * @return 成功时返回指向新栈的指针，如果 element_size 为0或内存分配失败，则返回 NULL。
 */
Stack* stack_create(size_t element_size);

/**
 * @brief 销毁一个栈并释放其所有节点和相关内存。
 *
 * @param p_stack 指向栈指针的指针。函数执行后，*p_stack 将被设置为 NULL。
 */
void stack_destroy(Stack** p_stack);

/**
 * @brief 将一个元素压入栈顶。
 *
 * @param stack 指向要操作的栈的指针。
 * @param element_data 指向要压入栈的元素数据的指针。
 * @return 成功压栈返回 true，如果内存分配失败或参数无效则返回 false。
 */
bool stack_push(Stack* stack, const void* element_data);

/**
 * @brief 从栈顶弹出一个元素。
 *
 * @param stack 指向要操作的栈的指针。
 * @param output_buffer 指向一个缓冲区的指针，用于接收弹出的元素数据。
 * @return 成功弹栈返回 true，如果栈为空或参数无效则返回 false。
 */
bool stack_pop(Stack* stack, void* output_buffer);

/**
 * @brief 查看栈顶元素，但不将其弹出。
 *
 * @param stack 指向要操作的栈的指针。
 * @param output_buffer 指向一个缓冲区的指针，用于接收栈顶元素的数据。
 * @return 成功查看返回 true，如果栈为空或参数无效则返回 false。
 */
bool stack_peek(const Stack* stack, void* output_buffer);

/**
 * @brief 检查栈是否为空。
 *
 * @param stack 指向要操作的栈的指针。
 * @return 如果栈为空或为NULL则返回 true，否则返回 false。
 */
bool stack_is_empty(const Stack* stack);

/**
 * @brief 获取栈中当前的元素数量。
 *
 * @param stack 指向要操作的栈的指针。
 * @return 返回栈中的元素数量。如果栈为NULL，返回0。
 */
size_t stack_get_size(const Stack* stack);

