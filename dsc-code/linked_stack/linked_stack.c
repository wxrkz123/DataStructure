#include "linked_stack.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definitions ---

// �ڲ��ڵ�ṹ�����û����ɼ�
typedef struct Node {
    void* data;         // ָ��Ϊ�ýڵ�Ԫ�ط�����ڴ�
    struct Node* next;  // ָ��ջ�е���һ���ڵ�
} Node;

// ��ʽջ��ʵ�ʹ���ṹ
struct LinkedStack {
    Node* top;              // ָ��ջ���Ľڵ�
    size_t element_size;    // ÿ��Ԫ�صĴ�С
    size_t size;            // ջ�е�ǰ��Ԫ������
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

    stack->top = NULL; // ��ʼ��ʱ��ջ��û�нڵ�
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

    // �������������ͷ�ÿһ���ڵ����������������
    while (current != NULL) {
        Node* temp = current;
        current = current->next; // �ƶ�����һ���ڵ�
        free(temp->data);        // �ͷŽڵ������
        free(temp);              // �ͷŽڵ㱾��
    }

    free(stack);      // ����ͷ�ջ�Ĺ���ṹ
    *p_stack = NULL;  // �����ⲿָ��ΪNULL
}

bool stack_push(Stack* stack, const void* element_data) {
    if (stack == NULL || element_data == NULL) {
        return false;
    }

    // 1. Ϊ�½ڵ�����ڴ�
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return false; // �ڴ����ʧ��
    }

    // 2. Ϊ�½ڵ�������������ڴ�
    new_node->data = malloc(stack->element_size);
    if (new_node->data == NULL) {
        free(new_node); // �����ѷ���Ľڵ㣬��ֹй©
        return false;
    }

    // 3. ���û����ݿ������½ڵ��������
    memcpy(new_node->data, element_data, stack->element_size);

    // 4. ���½ڵ����ӵ�ջ��
    new_node->next = stack->top; // �½ڵ����һ���ǵ�ǰ��ջ�� (��ջ��)
    stack->top = new_node;       // ����ջ��Ϊ�½ڵ�

    stack->size++;
    return true;
}

bool stack_pop(Stack* stack, void* output_buffer) {
    if (stack_is_empty(stack) || output_buffer == NULL) {
        return false;
    }

    // 1. �ݴ�ջ���ڵ�
    Node* node_to_pop = stack->top;

    // 2. �����ݴӽڵ㿽�����û��Ļ�����
    memcpy(output_buffer, node_to_pop->data, stack->element_size);

    // 3. ����ջ��ָ�룬ʹ��ָ����һ���ڵ�
    stack->top = node_to_pop->next;

    // 4. �ͷű������ڵ�������ڴ�
    free(node_to_pop->data);
    free(node_to_pop);

    stack->size--;
    return true;
}

bool stack_peek(const Stack* stack, void* output_buffer) {
    if (stack_is_empty(stack) || output_buffer == NULL) {
        return false;
    }

    // ֻ�追�����ݣ����޸��κ�ָ����ͷ��ڴ�
    memcpy(output_buffer, stack->top->data, stack->element_size);
    return true;
}

bool stack_is_empty(const Stack* stack) {
    if (stack == NULL) {
        return true;
    }
    return stack->top == NULL; // �� stack->size == 0
}

size_t stack_get_size(const Stack* stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}