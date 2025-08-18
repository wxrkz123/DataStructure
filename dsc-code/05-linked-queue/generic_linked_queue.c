#include "generic_linked_queue.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definitions ---

// 内部节点结构，对用户不可见
typedef struct Node {
    void* data;         // 指向为该节点元素动态分配的内存
    struct Node* next;  // 指向队列中的下一个节点
} Node;

// 链式队列的实际管理结构
struct LinkedQueue {
    Node* front;            // 指向队头的节点
    Node* rear;             // 指向队尾的节点
    size_t element_size;    // 每个元素的数据大小
    size_t size;            // 队列中当前的元素数量
};

// --- API Function Implementations ---

Queue* queue_create(size_t element_size) {
    if (element_size == 0) return NULL;
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->front = NULL; // 初始化时，队列为空，头尾指针都为NULL
    q->rear = NULL;
    q->element_size = element_size;
    q->size = 0;

    return q;
}

void queue_destroy(Queue** p_queue) {
    if (p_queue && *p_queue) {
        Queue* q = *p_queue;
        Node* current = q->front;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp->data); // 1. 释放节点的数据
            free(temp);       // 2. 释放节点本身
        }
        free(q);
        *p_queue = NULL;
    }
}

bool queue_enqueue(Queue* queue, const void* element_data) {
    /*
     * 函数功能: [工业级] 将一个新元素添加到链表队尾。
     * 教学案例 1 (常规情况): 队列中已有元素 A -> B。
     * front -> [A] -> [B] <- rear
     * 我们要入队一个新的元素 'C'。
     *
     * 教学案例 2 (特殊情况): 队列为空。
     * front -> NULL, rear -> NULL
     * 我们要入队第一个元素 'A'。
    */

    // 步骤 1: [防御性编程] 检查指针有效性。
    if (!queue || !element_data) {
        return false;
    }

    // 步骤 2: 为新节点本身分配内存。
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        return false; // 系统内存不足，入队失败
    }

    // 步骤 3: 为新节点的数据区分配内存。
    new_node->data = malloc(queue->element_size);
    if (!new_node->data) {
        free(new_node); // 如果数据区分配失败，必须清理已分配的节点，防止内存泄漏。
        return false;
    }

    // 步骤 4: 拷贝用户数据到新节点的数据区。
    memcpy(new_node->data, element_data, queue->element_size);
    new_node->next = NULL; // 新节点总是被加在队尾，所以它的next永远是NULL。

    // 步骤 5: [核心逻辑] 将新节点链接到队列尾部。
    if (queue_is_empty(queue)) {
        // --- 处理特殊情况：这是队列的第一个元素 ---
        // 案例 2: 队列为空，front和rear都为NULL。
        // front -> NULL
        // rear  -> NULL
        // new_node -> [A|next=NULL]
        // 让 front 和 rear 同时指向这个新节点。
        // front -> [A] <- rear
        queue->front = new_node;
        queue->rear = new_node;
    }
    else {
        // --- 处理常规情况：队列非空 ---
        // 案例 1: 队列为 A -> B。
        // rear 指向节点B: [B|next=NULL]
        // new_node: [C|next=NULL]
        // 关键一步: 让当前队尾节点(B)的next指针指向新节点(C)。
        // 链接后: [B|next=ptr_to_C] -> [C|next=NULL]
        queue->rear->next = new_node;
        // 然后，更新rear指针，让它指向新的队尾节点(C)。
        // A -> B -> [C] <- rear
        queue->rear = new_node;
    }

    // 步骤 6: 更新队列大小。
    queue->size++;
    return true;
}

bool queue_dequeue(Queue* queue, void* output_buffer) {
    /*
     * 函数功能: [工业级] 从链表队头移除一个元素。
     * 教学案例 1 (常规情况): 队列为 A -> B -> C。
     * front -> [A] -> [B] -> [C] <- rear
     * 我们要出队元素 'A'。
     *
     * 教学案例 2 (特殊情况): 队列中只有一个元素。
     * front -> [A] <- rear
     * 我们要出队元素 'A'。
    */

    // 步骤 1: [防御性编程] 检查指针有效性及队列是否为空。
    if (queue_is_empty(queue) || !output_buffer) {
        return false;
    }

    // 步骤 2: 创建一个临时指针，指向即将被移除的队头节点。
    // 案例 1 & 2: temp -> [A]
    Node* temp = queue->front;

    // 步骤 3: 拷贝队头节点的数据到用户的缓冲区。
    memcpy(output_buffer, temp->data, queue->element_size);

    // 步骤 4: [核心逻辑] 更新front指针，使其指向下一个节点。
    // 案例 1: 原front指向A，A的next指向B。
    //      新front = temp->next，所以新front指向了节点B。
    //      逻辑上队列变为: front -> [B] -> [C] <- rear
    queue->front = temp->next;

    // 步骤 5: [处理特殊情况] 如果更新后的front为NULL，说明队列变空了。
    // 案例 2: 原队列只有一个元素A，A的next是NULL。
    //      执行步骤4后，queue->front变为NULL。
    //      这表示队列中已无任何元素，此时必须同时更新rear指针也为NULL，
    //      以保持队列状态的一致性。
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // 步骤 6: 释放被移除节点的内存（数据区和节点本身）。
    // 案例 1 & 2: 释放 temp 指向的节点A的数据区和节点A本身。
    free(temp->data);
    free(temp);

    // 步骤 7: 更新队列大小。
    queue->size--;
    return true;
}

bool queue_peek(const Queue* queue, void* output_buffer) {
    if (queue_is_empty(queue) || !output_buffer) {
        return false;
    }
    // Peek操作非常简单：只拷贝队头的数据，不进行任何修改。
    memcpy(output_buffer, queue->front->data, queue->element_size);
    return true;
}

bool queue_is_empty(const Queue* queue) {
    if (!queue) return true;
    return queue->size == 0; // 或者判断 queue->front == NULL
}

size_t queue_get_size(const Queue* queue) {
    if (!queue) return 0;
    return queue->size;
}