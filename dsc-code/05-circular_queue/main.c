#include "generic_circular_queue.h"
#include <stdio.h>

// �������������ڴ�ӡ���е���ϸ�ڲ�״̬�����ڽ�ѧ
// ... (�˴�ʡ������һ����ȫ��ͬ�� print_queue_status ����) ...
void print_queue_status(const Queue* q, const char* title) {
    printf("--- %s ---\n", title);
    if (!q) {
        printf("����Ϊ NULL\n\n");
        return;
    }
    printf("����: %zu, ��С: %zu\n", queue_get_capacity(q), queue_get_size(q));
    printf("��? %s, ��? %s\n", queue_is_empty(q) ? "��" : "��", queue_is_full(q) ? "��" : "��");
    struct CircularQueue_Internal {
        void* data; size_t capacity; size_t element_size; size_t size;
        int front; int rear;
    };
    struct CircularQueue_Internal* q_internal = (struct CircularQueue_Internal*)q;
    printf("�ڲ�ָ��: front = %d, rear = %d\n\n", q_internal->front, q_internal->rear);
}


int main(void) {
    // 1. ����һ������Ϊ5����������
    Queue* q = queue_create(5, sizeof(int));
    print_queue_status(q, "1. ��ʼ״̬");

    // 2. ���3��Ԫ��
    for (int i = 10; i <= 30; i += 10) {
        printf("���: %d\n", i);
        queue_enqueue(q, &i);
    }
    print_queue_status(q, "2. ��� 10, 20, 30 ��");

    // 3. ����2��Ԫ��
    int dequeued_val;
    for (int i = 0; i < 2; i++) {
        if (queue_dequeue(q, &dequeued_val)) {
            printf("����: %d\n", dequeued_val);
        }
    }
    print_queue_status(q, "3. ��������Ԫ�غ�");

    // 4. ������ӣ�չʾ��ѭ��������
    struct CircularQueue_Internal {
        void* data; size_t capacity; size_t element_size; size_t size;
        int front; int rear;
    };
    printf("���� rear=%d, front=%d���ռ���������ǰ���ճ���\n", ((struct CircularQueue_Internal*)q)->rear, ((struct CircularQueue_Internal*)q)->front);
    printf("������� 40, 50, 60 ����������...\n");
    for (int i = 40; i <= 60; i += 10) {
        printf("���: %d\n", i);
        queue_enqueue(q, &i);
    }
    print_queue_status(q, "4. ��� 40, 50, 60 �� (����ѭ��)");

    // 5. �鿴��ͷ
    int peek_val;
    if (queue_peek(q, &peek_val)) {
        printf("�鿴��ͷԪ�� (ӦΪ30): %d\n\n", peek_val);
    }

    // 6. ������������ʱ���
    int overflow_val = 999;
    printf("������� %d ����������...\n", overflow_val);
    if (!queue_enqueue(q, &overflow_val)) {
        printf("���ʧ�ܣ�����Ԥ�ڡ�\n\n");
    }

    // 7. ȫ�����ӣ���ն���
    printf("������Ԫ�س���:\n");
    while (!queue_is_empty(q)) {
        queue_dequeue(q, &dequeued_val);
        printf("����: %d\n", dequeued_val);
    }
    print_queue_status(q, "7. ȫ�����Ӻ�");

    // 8. ���ٶ���
    queue_destroy(&q);
    printf("���������٣�ָ��Ϊ: %s\n", q == NULL ? "NULL" : "Not NULL");

    return 0;
}