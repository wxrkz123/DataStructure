// generic_circular_queue.c

#include "generic_circular_queue.h"
#include <stdlib.h>
#include <string.h>

// �ڲ��ṹ���屣�ֲ���
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
    // ��ҵ����� 1: ȷ��������Ԫ�ش�С��Ϊ0��
    // ����һ�������Ĳ�����������û��������û�д�С�Ķ�����������ġ�
    if (capacity == 0 || element_size == 0) {
        return NULL;
    }

    // Ϊ����ṹ������ڴ�
    Queue* q = (Queue*)malloc(sizeof(Queue));
    // ��ҵ����� 2: ��� malloc �Ƿ�ɹ���
    // ���ڴ����޵�ϵͳ�У�malloc ���ܻ�ʧ�ܡ�
    if (!q) {
        return NULL; // ���ʧ�ܣ��޷����������� NULL
    }

    // Ϊʵ�ʴ洢���ݵ���������ڴ�
    q->data = malloc(capacity * element_size);
    // ��ҵ����� 3: ���Ϊ�����������ڴ��Ƿ�ɹ���
    if (!q->data) {
        free(q); // �������������ʧ�ܣ������ͷ�֮ǰ�ѷ���Ĺ���ṹ�壬��ֹ�ڴ�й©��
        return NULL;
    }

    // ��ʼ�����г�Ա����
    q->capacity = capacity;
    q->element_size = element_size;
    q->size = 0;
    q->front = 0;
    q->rear = 0;

    return q;
}

void queue_destroy(Queue** p_queue) {
    // ��ҵ�����: �����ָ���ָ��ָ������ݶ�������Ч��
    // *p_queue ���ȷ�����ǲ����һ���Ѿ��� NULL ��ָ����н����á�
    if (p_queue && *p_queue) {
        free((*p_queue)->data); // 1. ���ͷ��ڲ�����������
        free(*p_queue);         // 2. ���ͷŶ��й���ṹ�屾��
        *p_queue = NULL;        // 3. ���ⲿ��ָ������ΪNULL�����Ƿ�ֹ������ָ�롱�����ʵ����
    }
}

bool queue_enqueue(Queue* queue, const void* element_data) {
    /*
     * ��������: [��ҵ��] ��һ����Ԫ����ӵ���β��
     * ��ѧ����: ����������һ������Ϊ4�Ķ��У���ǰ״̬���£�
     * capacity = 4, size = 2, front = 2, rear = 0
     * �������� (�߼���): [ (��) | (��) | A | B ]
     * �����е��߼�˳���� A -> B��
     * ��������Ҫ���һ���µ�Ԫ�� 'C'��
    */

    // ���� 1: [�����Ա��] ������д����ָ���Ƿ���Ч��
    // ��ҵ�����������κδ����ָ�붼��������Ч��(NULL)��
    // ������в����ڣ�����û���ṩҪ��ӵ����ݣ������޷����С�
    if (!queue || !element_data) {
        return false;
    }

    // ���� 2: �������Ƿ�������
    // ����: ��ǰ size=2, capacity=4��δ�������ͨ����
    if (queue->size == queue->capacity) {
        return false;
    }

    // ���� 3: ����Ҫ����Ԫ�ص������ڴ��ַ��
    // `(char*)` ����ת����Ϊ�˽��б�׼���ֽڼ�ָ��������
    // ����: rear = 0, element_size = sizeof(char) = 1 (����)��
    //      target_address = (char*)queue->data + (0 * 1) = �������ʼ��ַ��
    void* target_address = (char*)queue->data + (queue->rear * queue->element_size);

    // ���� 4: ʹ��memcpy���û����ݰ�ȫ�ؿ�����������ĵ�ַ��
    // ����: �� 'C' �����ݴ� element_data ��������������0��λ�á�
    //      �����Ϊ: [ C | (��) | A | B ]
    memcpy(target_address, element_data, queue->element_size);

    // ���� 5: ���¶�β rear ָ�룬ʵ�֡�ѭ������
    // ģ����(%)��ѭ�����е���ꡣ�����Զ������ƻء��������
    // ����: rear ԭ���� 0��
    //      �� rear = (0 + 1) % 4 = 1��
    //      rear ָ�����ڰ�ȫ��ָ������һ����λ������1��
    queue->rear = (queue->rear + 1) % queue->capacity;

    // ���� 6: ���¶��еĴ�С��
    // ����: size ԭ���� 2�����ڳɹ����ӵ� 3��
    queue->size++;

    return true; // ���в���ɹ����
}

bool queue_dequeue(Queue* queue, void* output_buffer) {
    /*
     * ��������: [��ҵ��] �Ӷ�ͷȡ��һ��Ԫ�ء�
     * ��ѧ����: �н���������ӣ����ڶ���״̬���£�
     * capacity = 4, size = 3, front = 2, rear = 1
     * �������� (�߼���): [ C | (��) | A | B ]
     * �����е��߼�˳���� A -> B -> C��
     * ��������Ҫ���Ӷ�ͷԪ�� 'A'��
    */

    // ���� 1: [�����Ա��] �������ָ���Ƿ���Ч��
    // ������в����ڣ������û�û���ṩ���ڽ������ݵĻ������������޷����С�
    if (!queue || !output_buffer) {
        return false;
    }

    // ���� 2: �������Ƿ�Ϊ�ա�
    // ����: ��ǰ size=3����Ϊ�գ����ͨ����
    if (queue->size == 0) {
        return false;
    }

    // ���� 3: �����ͷԪ�ص������ڴ��ַ��
    // ����: front = 2, element_size = 1 (����)��
    //      source_address = (char*)queue->data + (2 * 1) = ָ����������2�ĵ�ַ��
    void* source_address = (char*)queue->data + (queue->front * queue->element_size);

    // ���� 4: ʹ��memcpy����ͷ���ݰ�ȫ�ؿ������û��Ļ�������
    // ����: ����������2λ�õ� 'A' �����ݿ����� output_buffer��
    memcpy(output_buffer, source_address, queue->element_size);

    // ���� 5: ���¶�ͷ front ָ�룬ʵ�֡��߼�ɾ�����͡�ѭ������
    // ����: front ԭ���� 2��
    //      �� front = (2 + 1) % 4 = 3��
    //      front ָ������ָ���� 'B' (����3)��'A' ���߼����Ѿ����Ƴ��ˡ�
    queue->front = (queue->front + 1) % queue->capacity;

    // ���� 6: ���¶��еĴ�С��
    // ����: size ԭ���� 3�����ڰ�ȫ�ؼ��ٵ� 2��
    queue->size--;

    return true; // ���в���ɹ����
}

// void* queue_peek(const Queue* queue, void* output_buffer);

bool queue_peek(const Queue* queue, void* output_buffer) {
    /*
     * ��������: [��ҵ��] �鿴��ͷԪ�أ������ƶ��κ�ָ�롣
     * ��ѧ����: ����״̬��size = 2, front = 3, rear = 1
     * �������� (�߼���): [ C | (��) | A | B ]
     * �����߼�˳��: B -> C
     * ����Ҫ�鿴��ͷ�� 'B'��
    */

    // ���� 1: [�����Ա��] �������ָ���Ƿ���Ч��
    if (!queue || !output_buffer) {
        return false;
    }

    // ���� 2: �������Ƿ�Ϊ�ա�
    if (queue->size == 0) {
        return false;
    }

    // ���� 3: �����ͷԪ�صĵ�ַ��
    // ����: front = 3��source_address ָ����������3��λ�á�
    void* source_address = (char*)queue->data + (queue->front * queue->element_size);

    // ���� 4: �������ݵ��û���������
    // ����: �� 'B' �����ݿ����� output_buffer��
    memcpy(output_buffer, source_address, queue->element_size);

    // ** Peek �����ĺ���: ���˽��������޸� queue->front, queue->rear, �� queue->size��**

    return true;
}

// ����ĸ��������Ƚϼ򵥣���ͬ����Ҫ����NULL���
bool queue_is_empty(const Queue* queue) {
    if (!queue) return true; // һ�������ڵĶ��п�����Ϊ��
    return queue->size == 0;
}

bool queue_is_full(const Queue* queue) {
    if (!queue) return true; // һ�������ڵĶ��п������������Է�ֹ����ض�����в���
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