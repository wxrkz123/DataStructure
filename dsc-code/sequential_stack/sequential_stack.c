#include "sequential_stack.h"
#include <stdlib.h> // for malloc, free
#include <string.h> // for memcpy
#include <assert.h> // for assert

// --- Private Structure Definition ---
// ����ջ��ʵ���ڲ��ṹ�����û����ɼ���
struct Stack {
    void* data;          // ָ��洢���ݵ������ڴ�� (���ǵ�"����")
    size_t capacity;     // ջ����������Ԫ�ظ�����
    size_t element_size; // ÿ��Ԫ�صĴ�С���ֽڣ�
    int top;             // ջ��������-1 ��ʾ��ջ
};

// --- API Function Implementations ---

Stack* stack_create(size_t capacity, size_t element_size) {
    if (capacity == 0 || element_size == 0) {
        return NULL; // ��Ч����
    }

    // 1. Ϊջ�ṹ�屾������ڴ�
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL; // �ڴ����ʧ��
    }

    // 2. Ϊ�洢���ݵ���������ڴ�
    stack->data = malloc(capacity * element_size);
    if (stack->data == NULL) {
        free(stack); // �����ѷ����ջ�ṹ�壬��ֹ�ڴ�й©
        return NULL; // �ڴ����ʧ��
    }

    // 3. ��ʼ��ջ������
    stack->capacity = capacity;
    stack->element_size = element_size;
    stack->top = -1; // -1 �����ջ

    return stack;
}

void stack_destroy(Stack** p_stack) {
    if (p_stack == NULL || *p_stack == NULL) {
        return; // ��ָ�룬�������
    }

    free((*p_stack)->data);  // 1. �ͷ��������ڴ�
    free(*p_stack);          // 2. �ͷ�ջ�ṹ���ڴ�
    *p_stack = NULL;         // 3. ���ⲿָ����ΪNULL����ֹҰָ��
}

bool stack_push(Stack* stack, const void* element_data) {
    if (stack == NULL || element_data == NULL) {
        return false;
    }
    if (stack_is_full(stack)) {
        return false; // ջ�����޷�ѹ��
    }

    stack->top++;
    // ����Ҫ�����λ�õĵ�ַ
    // ʹ�� (char*) ����Ϊ�� void* ��ָ���������Ǳ�׼C
    // char* ���ֽ��ƶ�����Ϊ��ȫ
    // ����Ŀ���Ǽ������Ԫ���������ڴ���Ӧ�ô�ŵ�λ�á�
    void* target_address = (char*)stack->data + (stack->top * stack->element_size);
	
	// Stack* my_stack = stack_create(5, sizeof(int));
	// my_stack->data ��һ��ָ�������ڴ���ָ�� 20
	// my_stack->capacity �� 5
	// my_stack->element_size �� 4
	// my_stack->top �� -1
    // 
	// stack->top++; // �Ƚ� top ���ӣ�ָ����һ����λ
    // top => 0
    // top 0 => index
    
    // stack->top * stack->element_size �����ֽ�ƫ����
	// stack->element_size ��ÿ��Ԫ�صĴ�С 4
	// 0*4 => 

    // (char*)stack->data
	// C�����У�ָ�������ǻ������͵ģ�����������Ҫ�� void* ת��Ϊ char*��
    // ��Ϊ char �� 1 �ֽڵ����ͣ��������԰��ֽڽ���ƫ�ơ�
	// Pointer arithmetic
	// stack->data  void* , C������������Void* ����ָ������������void* + 1 �ǲ��Ϸ��ġ��������ƶ������ֽڣ�
    // char 1
   
	// ��ˣ������ǰ��κ�ָ�������ת��Ϊ char* ʱ�����Ǿ��൱�ڸ��߱������������ڰ�����ָ�뿴����һ��
	// �ֽ����������������Ϳ��԰��ֽڽ���ƫ���ˡ���ָ��һ�����ֽ����ݿ��ָ�룩
    // (char*)stack -> data + N �� ��ȡstack->data �ĵ�ַ����������ƶ� N���ֽ�
    //   void* target_address = (char*)stack->data + (stack->top * stack->element_size);

	// void* target_address = (char*)stack->data + 0
	// ȡջ����������ʼ��ַ stack->data����������һ���ֽ�ָ�룬����ƶ�0���ֽڣ��õ��µĵ�ַ
    // ����������Ԫ�ص�Ŀ���ŵ�ַ��


	// *target_address = element_data;
    // ���û����ݿ�����ջ���ڴ���
    memcpy(target_address, element_data, stack->element_size);

    return true;
}

bool stack_pop(Stack* stack, void* output_buffer) {
    if (stack == NULL || output_buffer == NULL) {
        return false;
    }
    if (stack_is_empty(stack)) {
        return false; // ջ�գ��޷�����
    }

    // ����ջ��Ԫ�صĵ�ַ
    void* source_address = (char*)stack->data + (stack->top * stack->element_size);
	// my_stack->data => 10,20,30,40,50
	// capacity = 5
	// element_size = 4
	// top = 4

    // int received_value;
	// bool success = stack_pop(my_stack, &received_value);



    // ��ջ�����ݿ������û��Ļ�����
    memcpy(output_buffer, source_address, stack->element_size);

    stack->top--;
    // �߼�ɾ����ͨ���ƶ�ָ��������һ��������Ч�������ǻ���ʱ��ȥ������

    return true;
}

bool stack_peek(Stack* stack, void* output_buffer) {
    if (stack == NULL || output_buffer == NULL) {
        return false;
    }
    if (stack_is_empty(stack)) {
        return false; // ջ�գ��޷��鿴
    }

    void* source_address = (char*)stack->data + (stack->top * stack->element_size);
    memcpy(output_buffer, source_address, stack->element_size);

    // �� pop ��Ψһ���𣺲��ƶ� top ָ��
    return true;
}

bool stack_is_empty(const Stack* stack) {
    if (stack == NULL) {
        return true; // ������
    }
    return stack->top == -1;
}

bool stack_is_full(const Stack* stack) {
    if (stack == NULL) {
        return false; // NULLջ����
    }
    // ע������ת���������з��ź��޷��������Ƚϵľ���
    return stack->top == (int)(stack->capacity - 1);
}

size_t stack_get_size(const Stack* stack) {
    if (stack == NULL) {
        return 0;
    }
    return (size_t)(stack->top + 1);
}

size_t stack_get_capacity(const Stack* stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->capacity;
}