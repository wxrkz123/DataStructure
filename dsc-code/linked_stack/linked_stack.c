#include "linked_stack.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definitions ---

// 内部节点结构，对用户不可见
typedef struct Node {
    void* data;         // 指向为该节点元素分配的内存
    struct Node* next;  // 指向栈中的下一个节点
} Node;

// 链式栈的实际管理结构
struct LinkedStack {
    Node* top;              // 指向栈顶的节点
    size_t element_size;    // 每个元素的大小
    size_t size;            // 栈中当前的元素数量
};

// --- API Function Implementations ---

Stack* stack_create(size_t element_size) {
    if (element_size == 0) {
        return NULL;
    }

    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->top = NULL; // 初始化时，栈顶没有节点
    stack->element_size = element_size;
    stack->size = 0;

    return stack;
}

void stack_destroy(Stack** p_stack) {
    if (p_stack == NULL || *p_stack == NULL) {
        return;
    }

    Stack* stack = *p_stack;
    Node* current = stack->top;

    // 遍历整个链表，释放每一个节点和它所包含的数据
    while (current != NULL) {
        Node* temp = current;
        current = current->next; // 移动到下一个节点
        free(temp->data);        // 释放节点的数据
        free(temp);              // 释放节点本身
    }

    free(stack);      // 最后释放栈的管理结构
    *p_stack = NULL;  // 设置外部指针为NULL
}

bool stack_push(Stack* stack, const void* element_data) {
    if (stack == NULL || element_data == NULL) {
        return false;
    }

    // 1. 为新节点分配内存
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return false; // 内存分配失败
    }

    // 2. 为新节点的数据区分配内存
    new_node->data = malloc(stack->element_size);
    if (new_node->data == NULL) {
        free(new_node); // 清理已分配的节点，防止泄漏
        return false;
    }

    // 3. 将用户数据拷贝到新节点的数据区
    memcpy(new_node->data, element_data, stack->element_size);

    // 4. 将新节点链接到栈顶
    new_node->next = stack->top; // 新节点的下一个是当前的栈顶 (旧栈顶)
    stack->top = new_node;       // 更新栈顶为新节点

    stack->size++;
    return true;
}

bool stack_pop(Stack* stack, void* output_buffer) {
    if (stack_is_empty(stack) || output_buffer == NULL) {
        return false;
    }

    // 1. 暂存栈顶节点
    Node* node_to_pop = stack->top;

    // 2. 将数据从节点拷贝到用户的缓冲区
    memcpy(output_buffer, node_to_pop->data, stack->element_size);

    // 3. 更新栈顶指针，使其指向下一个节点
    stack->top = node_to_pop->next;

    // 4. 释放被弹出节点的所有内存
    free(node_to_pop->data);
    free(node_to_pop);

    stack->size--;
    return true;
}

bool stack_peek(const Stack* stack, void* output_buffer) {
    if (stack_is_empty(stack) || output_buffer == NULL) {
        return false;
    }

    // 只需拷贝数据，不修改任何指针或释放内存
    memcpy(output_buffer, stack->top->data, stack->element_size);
    return true;
}

bool stack_is_empty(const Stack* stack) {
    if (stack == NULL) {
        return true;
    }
    return stack->top == NULL; // 或 stack->size == 0
}

size_t stack_get_size(const Stack* stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}