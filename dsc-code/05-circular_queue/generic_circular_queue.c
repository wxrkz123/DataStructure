// generic_circular_queue.c

#include "generic_circular_queue.h"
#include <stdlib.h>
#include <string.h>

// 内部结构定义保持不变
struct CircularQueue {
    void* data;
    size_t capacity;
    size_t element_size;
    size_t size;
    int front;
    int rear;
};

// --- API Function Implementations ---

Queue* queue_create(size_t capacity, size_t element_size) {
    // 工业级检查 1: 确保容量和元素大小不为0。
    // 这是一个基本的不变量，创建没有容量或没有大小的队列是无意义的。
    if (capacity == 0 || element_size == 0) {
        return NULL;
    }

    // 为管理结构体分配内存
    Queue* q = (Queue*)malloc(sizeof(Queue));
    // 工业级检查 2: 检查 malloc 是否成功。
    // 在内存受限的系统中，malloc 可能会失败。
    if (!q) {
        return NULL; // 如果失败，无法继续，返回 NULL
    }

    // 为实际存储数据的数组分配内存
    q->data = malloc(capacity * element_size);
    // 工业级检查 3: 检查为数据区分配内存是否成功。
    if (!q->data) {
        free(q); // 如果数据区分配失败，必须释放之前已分配的管理结构体，防止内存泄漏。
        return NULL;
    }

    // 初始化所有成员变量
    q->capacity = capacity;
    q->element_size = element_size;
    q->size = 0;
    q->front = 0;
    q->rear = 0;

    return q;
}

void queue_destroy(Queue** p_queue) {
    // 工业级检查: 传入的指针和指针指向的内容都必须有效。
    // *p_queue 检查确保我们不会对一个已经是 NULL 的指针进行解引用。
    if (p_queue && *p_queue) {
        free((*p_queue)->data); // 1. 先释放内部的数据数组
        free(*p_queue);         // 2. 再释放队列管理结构体本身
        *p_queue = NULL;        // 3. 将外部的指针设置为NULL，这是防止“悬空指针”的最佳实践。
    }
}

bool queue_enqueue(Queue* queue, const void* element_data) {
    /*
     * 函数功能: [工业级] 将一个新元素添加到队尾。
     * 教学案例: 假设我们有一个容量为4的队列，当前状态如下：
     * capacity = 4, size = 2, front = 2, rear = 0
     * 数组内容 (逻辑上): [ (空) | (空) | A | B ]
     * 队列中的逻辑顺序是 A -> B。
     * 我们现在要入队一个新的元素 'C'。
    */

    // 步骤 1: [防御性编程] 检查所有传入的指针是否有效。
    // 工业代码必须假设任何传入的指针都可能是无效的(NULL)。
    // 如果队列不存在，或者没有提供要入队的数据，操作无法进行。
    if (!queue || !element_data) {
        return false;
    }

    // 步骤 2: 检查队列是否已满。
    // 案例: 当前 size=2, capacity=4，未满，检查通过。
    if (queue->size == queue->capacity) {
        return false;
    }

    // 步骤 3: 计算要插入元素的物理内存地址。
    // `(char*)` 类型转换是为了进行标准的字节级指针算术。
    // 案例: rear = 0, element_size = sizeof(char) = 1 (假设)。
    //      target_address = (char*)queue->data + (0 * 1) = 数组的起始地址。
    void* target_address = (char*)queue->data + (queue->rear * queue->element_size);

    // 步骤 4: 使用memcpy将用户数据安全地拷贝到计算出的地址。
    // 案例: 将 'C' 的数据从 element_data 拷贝到数组索引0的位置。
    //      数组变为: [ C | (空) | A | B ]
    memcpy(target_address, element_data, queue->element_size);

    // 步骤 5: 更新队尾 rear 指针，实现“循环”。
    // 模运算(%)是循环队列的灵魂。它能自动处理“绕回”的情况。
    // 案例: rear 原本是 0。
    //      新 rear = (0 + 1) % 4 = 1。
    //      rear 指针现在安全地指向了下一个空位，索引1。
    queue->rear = (queue->rear + 1) % queue->capacity;

    // 步骤 6: 更新队列的大小。
    // 案例: size 原本是 2，现在成功增加到 3。
    queue->size++;

    return true; // 所有步骤成功完成
}

bool queue_dequeue(Queue* queue, void* output_buffer) {
    /*
     * 函数功能: [工业级] 从队头取出一个元素。
     * 教学案例: 承接上面的例子，现在队列状态如下：
     * capacity = 4, size = 3, front = 2, rear = 1
     * 数组内容 (逻辑上): [ C | (空) | A | B ]
     * 队列中的逻辑顺序是 A -> B -> C。
     * 我们现在要出队队头元素 'A'。
    */

    // 步骤 1: [防御性编程] 检查所有指针是否有效。
    // 如果队列不存在，或者用户没有提供用于接收数据的缓冲区，操作无法进行。
    if (!queue || !output_buffer) {
        return false;
    }

    // 步骤 2: 检查队列是否为空。
    // 案例: 当前 size=3，不为空，检查通过。
    if (queue->size == 0) {
        return false;
    }

    // 步骤 3: 计算队头元素的物理内存地址。
    // 案例: front = 2, element_size = 1 (假设)。
    //      source_address = (char*)queue->data + (2 * 1) = 指向数组索引2的地址。
    void* source_address = (char*)queue->data + (queue->front * queue->element_size);

    // 步骤 4: 使用memcpy将队头数据安全地拷贝到用户的缓冲区。
    // 案例: 将数组索引2位置的 'A' 的数据拷贝到 output_buffer。
    memcpy(output_buffer, source_address, queue->element_size);

    // 步骤 5: 更新队头 front 指针，实现“逻辑删除”和“循环”。
    // 案例: front 原本是 2。
    //      新 front = (2 + 1) % 4 = 3。
    //      front 指针现在指向了 'B' (索引3)，'A' 在逻辑上已经被移除了。
    queue->front = (queue->front + 1) % queue->capacity;

    // 步骤 6: 更新队列的大小。
    // 案例: size 原本是 3，现在安全地减少到 2。
    queue->size--;

    return true; // 所有步骤成功完成
}

// void* queue_peek(const Queue* queue, void* output_buffer);

bool queue_peek(const Queue* queue, void* output_buffer) {
    /*
     * 函数功能: [工业级] 查看队头元素，但不移动任何指针。
     * 教学案例: 队列状态：size = 2, front = 3, rear = 1
     * 数组内容 (逻辑上): [ C | (空) | A | B ]
     * 队列逻辑顺序: B -> C
     * 我们要查看队头的 'B'。
    */

    // 步骤 1: [防御性编程] 检查所有指针是否有效。
    if (!queue || !output_buffer) {
        return false;
    }

    // 步骤 2: 检查队列是否为空。
    if (queue->size == 0) {
        return false;
    }

    // 步骤 3: 计算队头元素的地址。
    // 案例: front = 3。source_address 指向数组索引3的位置。
    void* source_address = (char*)queue->data + (queue->front * queue->element_size);

    // 步骤 4: 拷贝数据到用户缓冲区。
    // 案例: 将 'B' 的数据拷贝到 output_buffer。
    memcpy(output_buffer, source_address, queue->element_size);

    // ** Peek 操作的核心: 到此结束。不修改 queue->front, queue->rear, 或 queue->size。**

    return true;
}

// 其余的辅助函数比较简单，但同样需要进行NULL检查
bool queue_is_empty(const Queue* queue) {
    if (!queue) return true; // 一个不存在的队列可以视为空
    return queue->size == 0;
}

bool queue_is_full(const Queue* queue) {
    if (!queue) return true; // 一个不存在的队列可以视作满，以防止错误地对其进行操作
    return queue->size == queue->capacity;
}

size_t queue_get_size(const Queue* queue) {
    if (!queue) return 0;
    return queue->size;
}

size_t queue_get_capacity(const Queue* queue) {
    if (!queue) return 0;
    return queue->capacity;
}