#include "shared_stack.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definition ---
struct SharedStack {
    void* data;          // 指向共享内存区域的指针
    size_t capacity;     // 总容量 (元素个数)
    size_t element_size; // 每个元素的大小
    int top1;            // 栈1的栈顶索引
    int top2;            // 栈2的栈顶索引
};

// --- API Function Implementations ---

Stack* stack_create(size_t total_capacity, size_t element_size) {
    if (total_capacity == 0 || element_size == 0) {
        return NULL;
    }

    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        return NULL;
    }

    s->data = malloc(total_capacity * element_size);
    if (!s->data) {
        free(s);
        return NULL;
    }

    s->capacity = total_capacity;
    s->element_size = element_size;
    s->top1 = -1; // 栈1从头开始，-1为空
    s->top2 = (int)total_capacity; // 栈2从尾开始，capacity为空

    return s;
}

void stack_destroy(Stack** p_stack) {
    if (p_stack && *p_stack) {
        free((*p_stack)->data);
        free(*p_stack);
        *p_stack = NULL;
    }
}

bool stack_is_full(const Stack* stack) {
    if (!stack) {
        return true; // 无效栈视作满
    }
    return stack->top1 + 1 == stack->top2;
}

bool stack_is_empty(const Stack* stack, StackNumber num) {
    if (!stack) {
        return true; // 无效栈视作空
    }
    if (num == STACK_ONE) {
        return stack->top1 == -1;
    }
    else { // STACK_TWO
        return stack->top2 == (int)stack->capacity;
    }
}

bool stack_push(Stack* stack, StackNumber num, const void* element_data) {
    if (!stack || !element_data || stack_is_full(stack)) {
        return false;
    }

    void* target_address;
    if (num == STACK_ONE) {
        stack->top1++;
        target_address = (char*)stack->data + (stack->top1 * stack->element_size);
    }
    else { // STACK_TWO
        stack->top2--;
        target_address = (char*)stack->data + (stack->top2 * stack->element_size);
    }

    memcpy(target_address, element_data, stack->element_size);
    return true;
}

bool stack_pop(Stack* stack, StackNumber num, void* output_buffer) {
    if (!stack || !output_buffer || stack_is_empty(stack, num)) {
        return false;
    }

    void* source_address;
    if (num == STACK_ONE) {
        source_address = (char*)stack->data + (stack->top1 * stack->element_size);
        memcpy(output_buffer, source_address, stack->element_size);
        stack->top1--;
    }
    else { // STACK_TWO
        source_address = (char*)stack->data + (stack->top2 * stack->element_size);
        memcpy(output_buffer, source_address, stack->element_size);
        stack->top2++;
    }
    return true;
}

bool stack_peek(const Stack* stack, StackNumber num, void* output_buffer) {
    if (!stack || !output_buffer || stack_is_empty(stack, num)) {
        return false;
    }

    void* source_address;
    if (num == STACK_ONE) {
        source_address = (char*)stack->data + (stack->top1 * stack->element_size);
    }
    else { // STACK_TWO
        source_address = (char*)stack->data + (stack->top2 * stack->element_size);
    }
    memcpy(output_buffer, source_address, stack->element_size);
    return true;
}

size_t stack_get_size(const Stack* stack, StackNumber num) {
    if (!stack) {
        return 0;
    }
    if (num == STACK_ONE) {
        return (size_t)(stack->top1 + 1);
    }
    else { // STACK_TWO
        return stack->capacity - stack->top2;

		// Stack* ss = stack_create(10, sizeof(int));
		// ss-> capacity = 10;
		// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9  => index
		// top1 = -1; // 栈1从头开始，-1为空
		// top2 = 10; // 栈2从尾开始，capacity为空
		// Index:  0  1  2  3  4  5  6  7  8  9
		// Array: [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
		//                                       top2 (is at 10)
		// size = capacity - top2;
		// size = 10 - 10 = 0
        // size = 0
        // e.g 99
        // stack_push()
        // top2 10->9 top2--
        // Index:  0  1  2  3  4  5  6  7  8  9
        // Array: [ ][ ][ ][ ][ ][ ][ ][ ][ ][99]
		// 								   top2 (is at 9)

		// size = capacity - top2;
        // size = 10 - 9 = 1
		// size = 1

    }
}

size_t stack_get_total_capacity(const Stack* stack) {
    if (!stack) {
        return 0;
    }
    return stack->capacity;
}