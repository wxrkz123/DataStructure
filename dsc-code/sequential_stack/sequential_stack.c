#include "sequential_stack.h"
#include <stdlib.h> // for malloc, free
#include <string.h> // for memcpy
#include <assert.h> // for assert

// --- Private Structure Definition ---
// 这是栈的实际内部结构，对用户不可见。
struct Stack {
    void* data;          // 指向存储数据的连续内存块 (我们的"数组")
    size_t capacity;     // 栈的总容量（元素个数）
    size_t element_size; // 每个元素的大小（字节）
    int top;             // 栈顶索引，-1 表示空栈
};

// --- API Function Implementations ---

Stack* stack_create(size_t capacity, size_t element_size) {
    if (capacity == 0 || element_size == 0) {
        return NULL; // 无效参数
    }

    // 1. 为栈结构体本身分配内存
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL; // 内存分配失败
    }

    // 2. 为存储数据的数组分配内存
    stack->data = malloc(capacity * element_size);
    if (stack->data == NULL) {
        free(stack); // 清理已分配的栈结构体，防止内存泄漏
        return NULL; // 内存分配失败
    }

    // 3. 初始化栈的属性
    stack->capacity = capacity;
    stack->element_size = element_size;
    stack->top = -1; // -1 代表空栈

    return stack;
}

void stack_destroy(Stack** p_stack) {
    if (p_stack == NULL || *p_stack == NULL) {
        return; // 空指针，无需操作
    }

    free((*p_stack)->data);  // 1. 释放数据区内存
    free(*p_stack);          // 2. 释放栈结构体内存
    *p_stack = NULL;         // 3. 将外部指针置为NULL，防止野指针
}

bool stack_push(Stack* stack, const void* element_data) {
    if (stack == NULL || element_data == NULL) {
        return false;
    }
    if (stack_is_full(stack)) {
        return false; // 栈满，无法压入
    }

    stack->top++;
    // 计算要插入的位置的地址
    // 使用 (char*) 是因为对 void* 的指针算术不是标准C
    // char* 按字节移动，最为安全
    // 它的目标是计算出新元素在物理内存中应该存放的位置。
    void* target_address = (char*)stack->data + (stack->top * stack->element_size);
	
	// Stack* my_stack = stack_create(5, sizeof(int));
	// my_stack->data 是一个指向连续内存块的指针 20
	// my_stack->capacity 是 5
	// my_stack->element_size 是 4
	// my_stack->top 是 -1
    // 
	// stack->top++; // 先将 top 增加，指向下一个空位
    // top => 0
    // top 0 => index
    
    // stack->top * stack->element_size 计算字节偏移量
	// stack->element_size 是每个元素的大小 4
	// 0*4 => 

    // (char*)stack->data
	// C语言中，指针算术是基于类型的，所以我们需要将 void* 转换为 char*，
    // 因为 char 是 1 字节的类型，这样可以按字节进行偏移。
	// Pointer arithmetic
	// stack->data  void* , C语言里，不允许对Void* 进行指针算术操作，void* + 1 是不合法的。到底是移动多少字节？
    // char 1
   
	// 因此，当我们把任何指针的类型转换为 char* 时，我们就相当于告诉编译器：请现在把整个指针看看作一个
	// 字节数组来处理，这样就可以按字节进行偏移了。（指向一个单字节数据块的指针）
    // (char*)stack -> data + N ： 获取stack->data 的地址，并且向后移动 N个字节
    //   void* target_address = (char*)stack->data + (stack->top * stack->element_size);

	// void* target_address = (char*)stack->data + 0
	// 取栈数据区的起始地址 stack->data，把它当作一个字节指针，向后移动0个字节，得到新的地址
    // 就是我们新元素的目标存放地址。


	// *target_address = element_data;
    // 将用户数据拷贝到栈的内存中
    memcpy(target_address, element_data, stack->element_size);

    return true;
}

bool stack_pop(Stack* stack, void* output_buffer) {
    if (stack == NULL || output_buffer == NULL) {
        return false;
    }
    if (stack_is_empty(stack)) {
        return false; // 栈空，无法弹出
    }

    // 计算栈顶元素的地址
    void* source_address = (char*)stack->data + (stack->top * stack->element_size);
	// my_stack->data => 10,20,30,40,50
	// capacity = 5
	// element_size = 4
	// top = 4

    // int received_value;
	// bool success = stack_pop(my_stack, &received_value);



    // 将栈顶数据拷贝到用户的缓冲区
    memcpy(output_buffer, source_address, stack->element_size);

    stack->top--;
    // 逻辑删除：通过移动指针来宣告一块数据无效，而不是花费时间去清理它

    return true;
}

bool stack_peek(Stack* stack, void* output_buffer) {
    if (stack == NULL || output_buffer == NULL) {
        return false;
    }
    if (stack_is_empty(stack)) {
        return false; // 栈空，无法查看
    }

    void* source_address = (char*)stack->data + (stack->top * stack->element_size);
    memcpy(output_buffer, source_address, stack->element_size);

    // 与 pop 的唯一区别：不移动 top 指针
    return true;
}

bool stack_is_empty(const Stack* stack) {
    if (stack == NULL) {
        return true; // 视作空
    }
    return stack->top == -1;
}

bool stack_is_full(const Stack* stack) {
    if (stack == NULL) {
        return false; // NULL栈不满
    }
    // 注意类型转换，避免有符号和无符号整数比较的警告
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