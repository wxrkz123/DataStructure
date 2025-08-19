#include "generic_deque.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definition ---
// 与循环队列的结构完全相同，只是对 front 和 rear 的操作方式更多样
struct Deque {
    void* data;
    size_t capacity;
    size_t element_size;
    size_t size;
    int front; // 指向队头元素的索引
    int rear;  // 指向队尾元素的后一个位置的索引
};

// --- API Function Implementations ---

Deque* deque_create(size_t capacity, size_t element_size) {
    if (capacity == 0 || element_size == 0) return NULL;
    Deque* dq = (Deque*)malloc(sizeof(Deque));
    if (!dq) return NULL;
    dq->data = malloc(capacity * element_size);
    if (!dq->data) {
        free(dq);
        return NULL;
    }
    dq->capacity = capacity;
    dq->element_size = element_size;
    dq->size = 0;
    dq->front = 0;
    dq->rear = 0;
    return dq;
}

void deque_destroy(Deque** p_deque) {
    if (p_deque && *p_deque) {
        free((*p_deque)->data);
        free(*p_deque);
        *p_deque = NULL;
    }
}

bool deque_is_empty(const Deque* dq) { return !dq || dq->size == 0; }
bool deque_is_full(const Deque* dq) { return !dq || dq->size == dq->capacity; }
size_t deque_get_size(const Deque* dq) { return dq ? dq->size : 0; }


bool deque_push_back(Deque* dq, const void* element_data) {
    /*
     * 函数功能: 在队尾添加元素 (与循环队列的 enqueue 完全相同)
    */
    // 1. 检查
    if (!dq || !element_data || deque_is_full(dq)) {
        return false;
    }

    // 2. 拷贝数据到 rear 指向的位置
    // 案例: rear=4。数据 'C' 被拷贝到索引4。
    // 数组: [ | | A | B | C ]
    void* target_address = (char*)dq->data + dq->rear * dq->element_size;
    memcpy(target_address, element_data, dq->element_size);

    // 3. 更新 rear 指针 (向前移动并循环)
    // 案例: 新 rear = (4 + 1) % 5 = 0。rear 指针绕回到了数组开头。
    dq->rear = (dq->rear + 1) % dq->capacity;

    // 4. 更新大小
    dq->size++;
    return true;
}

bool deque_push_front(Deque* dq, const void* element_data) {
    /*
     * capacity 5 
     * size 2
	 * front 2 => 指向 A
	 * rear 4 => 指向 队尾元素 B 的后一个位置
	 * index:  0 1  2   3  4
	 * Array: [ | | A | B | ]
     * A -> B
	 * push_front(X)
	 * X -> A -> B
	 * new front  index 1 => 指向 X
     * 
     * 
    */
    // 1. 检查
    if (!dq || !element_data || deque_is_full(dq)) {
        return false;
    }

    /*
    * capacity 5
    * size 2
    * front 2 => 指向 A
    * rear 4 => 指向 队尾元素 B 的后一个位置
    * index:  0 1  2   3  4
    * Array: [ | | A | B | ]
    * A -> B
    * push_front(X)
    * X -> A -> B
    * new front  index 1 => 指向 X
    *
    * index:  0 1  2   3  4
    * Array: [| | A | B | ]
    * 
    * front = 2
	* 2 - 1 = 1
	* new_front = (old_front - 1 + capacity) % capacity
    * 
    * dq->front = 2
	* dq->capacity = 5
	* new_front = (2 - 1 + 5) % 5 = 1
    * 甲鱼的臀部――龟腚 ―― 规定
    * 
   */
    dq->front = (dq->front - 1 + dq->capacity) % dq->capacity;
	// index : 0    1   2   3   4
	// Array: [ A | B | C |   |   ]
    //          ^           ^
	//		  front        rear
	// new_front = (dq->front - 1) % dq->capacity

    // front 0
	// capacity 5
	// new_front = (0 - 1) % 5
	// -1 % 5
    // 取决于编译器
	// C99 , c11 ,c17 都是 -1 % 5 = -1
	// a % b = a
	// (x + N) % N and x % N 对于正数x来说是等价的，但在这里对于负数x来说，(x + N) % N能够得到一个非负结果。
	// 在C语言中，凡是能够涉及到可能产生负数的循环数组索引计算 都需要使用 (index - 1 + capacity) % capacity


    void* target_address = (char*)dq->data + dq->front * dq->element_size;
    memcpy(target_address, element_data, dq->element_size);

    // 4. 更新大小
    dq->size++;
    return true;
}

bool deque_pop_front(Deque* dq, void* output_buffer) {
    /*
     * 函数功能: 从队头移除元素 (与循环队列的 dequeue 完全相同)。
     * 教学案例: capacity=5, size=3, front=1, rear=4
     * 数组内容: [ | X | A | B | ]
     *              ^           ^
     *            front       rear
     * 我们要 pop_front()
    */
    // 1. 检查
    if (!dq || !output_buffer || deque_is_empty(dq)) {
        return false;
    }

    // 2. 从 front 指向的位置拷贝数据
    // 案例: front=1。将 'X' 拷贝到 output_buffer。
    void* source_address = (char*)dq->data + dq->front * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);

    // 3. 更新 front 指针 (向前移动并循环)
    // 案例: 新 front = (1 + 1) % 5 = 2。front 指针现在指向了 'A'。
    dq->front = (dq->front + 1) % dq->capacity;

    // 4. 更新大小
    dq->size--;
    return true;
}

bool deque_pop_back(Deque* dq, void* output_buffer) {
    /*
     * 函数功能: 从队尾移除元素 (这是双端队列的新功能)。
     * 教学案例: capacity=5, size=2, front=2, rear=4
     * 数组内容: [ | | A | B |     ]
     *                ^        ^
     *              front    rear
     * 我们要 pop_back()
    */
    // 1. 检查
    if (!dq || !output_buffer || deque_is_empty(dq)) {
        return false;
    }

    // 2. [核心逻辑] 更新 rear 指针 (向后移动并循环)
    // rear 指向的是队尾元素的“后一个”位置，所以真正的队尾元素在 rear-1。
    // 我们先将 rear 指针移回队尾元素的位置。
    // 案例: rear=4, capacity=5。
    //      新 rear = (4 - 1 + 5) % 5 = 8 % 5 = 3。
    dq->rear = (dq->rear - 1 + dq->capacity) % dq->capacity;

    // 3. 从新的 rear 指向的位置拷贝数据
    // 案例: 新 rear=3。将队尾元素 'B' 拷贝到 output_buffer。
    void* source_address = (char*)dq->data + dq->rear * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);

    // 4. 更新大小
    dq->size--;
    return true;
}

bool deque_peek_front(const Deque* dq, void* output_buffer) {
    if (!dq || !output_buffer || deque_is_empty(dq)) return false;
    void* source_address = (char*)dq->data + dq->front * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);
    return true;
}

bool deque_peek_back(const Deque* dq, void* output_buffer) {
    if (!dq || !output_buffer || deque_is_empty(dq)) return false;
    // 真正的队尾元素在 rear 的前一个位置
    int last_element_index = (dq->rear - 1 + dq->capacity) % dq->capacity;
    void* source_address = (char*)dq->data + last_element_index * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);
    return true;
}