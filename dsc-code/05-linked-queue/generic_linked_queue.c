#include "generic_linked_queue.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definitions ---

// �ڲ��ڵ�ṹ�����û����ɼ�
typedef struct Node {
    void* data;         // ָ��Ϊ�ýڵ�Ԫ�ض�̬������ڴ�
    struct Node* next;  // ָ������е���һ���ڵ�
} Node;

// ��ʽ���е�ʵ�ʹ���ṹ
struct LinkedQueue {
    Node* front;            // ָ���ͷ�Ľڵ�
    Node* rear;             // ָ���β�Ľڵ�
    size_t element_size;    // ÿ��Ԫ�ص����ݴ�С
    size_t size;            // �����е�ǰ��Ԫ������
};

// --- API Function Implementations ---

Queue* queue_create(size_t element_size) {
    if (element_size == 0) return NULL;
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->front = NULL; // ��ʼ��ʱ������Ϊ�գ�ͷβָ�붼ΪNULL
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
            free(temp->data); // 1. �ͷŽڵ������
            free(temp);       // 2. �ͷŽڵ㱾��
        }
        free(q);
        *p_queue = NULL;
    }
}

bool queue_enqueue(Queue* queue, const void* element_data) {
    /*
     * ��������: [��ҵ��] ��һ����Ԫ����ӵ������β��
     * ��ѧ���� 1 (�������): ����������Ԫ�� A -> B��
     * front -> [A] -> [B] <- rear
     * ����Ҫ���һ���µ�Ԫ�� 'C'��
     *
     * ��ѧ���� 2 (�������): ����Ϊ�ա�
     * front -> NULL, rear -> NULL
     * ����Ҫ��ӵ�һ��Ԫ�� 'A'��
    */

    // ���� 1: [�����Ա��] ���ָ����Ч�ԡ�
    if (!queue || !element_data) {
        return false;
    }

    // ���� 2: Ϊ�½ڵ㱾������ڴ档
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        return false; // ϵͳ�ڴ治�㣬���ʧ��
    }

    // ���� 3: Ϊ�½ڵ�������������ڴ档
    new_node->data = malloc(queue->element_size);
    if (!new_node->data) {
        free(new_node); // �������������ʧ�ܣ����������ѷ���Ľڵ㣬��ֹ�ڴ�й©��
        return false;
    }

    // ���� 4: �����û����ݵ��½ڵ����������
    memcpy(new_node->data, element_data, queue->element_size);
    new_node->next = NULL; // �½ڵ����Ǳ����ڶ�β����������next��Զ��NULL��

    // ���� 5: [�����߼�] ���½ڵ����ӵ�����β����
    if (queue_is_empty(queue)) {
        // --- ����������������Ƕ��еĵ�һ��Ԫ�� ---
        // ���� 2: ����Ϊ�գ�front��rear��ΪNULL��
        // front -> NULL
        // rear  -> NULL
        // new_node -> [A|next=NULL]
        // �� front �� rear ͬʱָ������½ڵ㡣
        // front -> [A] <- rear
        queue->front = new_node;
        queue->rear = new_node;
    }
    else {
        // --- ��������������зǿ� ---
        // ���� 1: ����Ϊ A -> B��
        // rear ָ��ڵ�B: [B|next=NULL]
        // new_node: [C|next=NULL]
        // �ؼ�һ��: �õ�ǰ��β�ڵ�(B)��nextָ��ָ���½ڵ�(C)��
        // ���Ӻ�: [B|next=ptr_to_C] -> [C|next=NULL]
        queue->rear->next = new_node;
        // Ȼ�󣬸���rearָ�룬����ָ���µĶ�β�ڵ�(C)��
        // A -> B -> [C] <- rear
        queue->rear = new_node;
    }

    // ���� 6: ���¶��д�С��
    queue->size++;
    return true;
}

bool queue_dequeue(Queue* queue, void* output_buffer) {
    /*
     * ��������: [��ҵ��] �������ͷ�Ƴ�һ��Ԫ�ء�
     * ��ѧ���� 1 (�������): ����Ϊ A -> B -> C��
     * front -> [A] -> [B] -> [C] <- rear
     * ����Ҫ����Ԫ�� 'A'��
     *
     * ��ѧ���� 2 (�������): ������ֻ��һ��Ԫ�ء�
     * front -> [A] <- rear
     * ����Ҫ����Ԫ�� 'A'��
    */

    // ���� 1: [�����Ա��] ���ָ����Ч�Լ������Ƿ�Ϊ�ա�
    if (queue_is_empty(queue) || !output_buffer) {
        return false;
    }

    // ���� 2: ����һ����ʱָ�룬ָ�򼴽����Ƴ��Ķ�ͷ�ڵ㡣
    // ���� 1 & 2: temp -> [A]
    Node* temp = queue->front;

    // ���� 3: ������ͷ�ڵ�����ݵ��û��Ļ�������
    memcpy(output_buffer, temp->data, queue->element_size);

    // ���� 4: [�����߼�] ����frontָ�룬ʹ��ָ����һ���ڵ㡣
    // ���� 1: ԭfrontָ��A��A��nextָ��B��
    //      ��front = temp->next��������frontָ���˽ڵ�B��
    //      �߼��϶��б�Ϊ: front -> [B] -> [C] <- rear
    queue->front = temp->next;

    // ���� 5: [�����������] ������º��frontΪNULL��˵�����б���ˡ�
    // ���� 2: ԭ����ֻ��һ��Ԫ��A��A��next��NULL��
    //      ִ�в���4��queue->front��ΪNULL��
    //      ���ʾ�����������κ�Ԫ�أ���ʱ����ͬʱ����rearָ��ҲΪNULL��
    //      �Ա��ֶ���״̬��һ���ԡ�
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // ���� 6: �ͷű��Ƴ��ڵ���ڴ棨�������ͽڵ㱾����
    // ���� 1 & 2: �ͷ� temp ָ��Ľڵ�A���������ͽڵ�A����
    free(temp->data);
    free(temp);

    // ���� 7: ���¶��д�С��
    queue->size--;
    return true;
}

bool queue_peek(const Queue* queue, void* output_buffer) {
    if (queue_is_empty(queue) || !output_buffer) {
        return false;
    }
    // Peek�����ǳ��򵥣�ֻ������ͷ�����ݣ��������κ��޸ġ�
    memcpy(output_buffer, queue->front->data, queue->element_size);
    return true;
}

bool queue_is_empty(const Queue* queue) {
    if (!queue) return true;
    return queue->size == 0; // �����ж� queue->front == NULL
}

size_t queue_get_size(const Queue* queue) {
    if (!queue) return 0;
    return queue->size;
}