#include "generic_deque.h"
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definition ---
// ��ѭ�����еĽṹ��ȫ��ͬ��ֻ�Ƕ� front �� rear �Ĳ�����ʽ������
struct Deque {
    void* data;
    size_t capacity;
    size_t element_size;
    size_t size;
    int front; // ָ���ͷԪ�ص�����
    int rear;  // ָ���βԪ�صĺ�һ��λ�õ�����
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
     * ��������: �ڶ�β���Ԫ�� (��ѭ�����е� enqueue ��ȫ��ͬ)
    */
    // 1. ���
    if (!dq || !element_data || deque_is_full(dq)) {
        return false;
    }

    // 2. �������ݵ� rear ָ���λ��
    // ����: rear=4������ 'C' ������������4��
    // ����: [ | | A | B | C ]
    void* target_address = (char*)dq->data + dq->rear * dq->element_size;
    memcpy(target_address, element_data, dq->element_size);

    // 3. ���� rear ָ�� (��ǰ�ƶ���ѭ��)
    // ����: �� rear = (4 + 1) % 5 = 0��rear ָ���ƻص������鿪ͷ��
    dq->rear = (dq->rear + 1) % dq->capacity;

    // 4. ���´�С
    dq->size++;
    return true;
}

bool deque_push_front(Deque* dq, const void* element_data) {
    /*
     * capacity 5 
     * size 2
	 * front 2 => ָ�� A
	 * rear 4 => ָ�� ��βԪ�� B �ĺ�һ��λ��
	 * index:  0 1  2   3  4
	 * Array: [ | | A | B | ]
     * A -> B
	 * push_front(X)
	 * X -> A -> B
	 * new front  index 1 => ָ�� X
     * 
     * 
    */
    // 1. ���
    if (!dq || !element_data || deque_is_full(dq)) {
        return false;
    }

    /*
    * capacity 5
    * size 2
    * front 2 => ָ�� A
    * rear 4 => ָ�� ��βԪ�� B �ĺ�һ��λ��
    * index:  0 1  2   3  4
    * Array: [ | | A | B | ]
    * A -> B
    * push_front(X)
    * X -> A -> B
    * new front  index 1 => ָ�� X
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
    * ������β��������� ���� �涨
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
    // ȡ���ڱ�����
	// C99 , c11 ,c17 ���� -1 % 5 = -1
	// a % b = a
	// (x + N) % N and x % N ��������x��˵�ǵȼ۵ģ�����������ڸ���x��˵��(x + N) % N�ܹ��õ�һ���Ǹ������
	// ��C�����У������ܹ��漰�����ܲ���������ѭ�������������� ����Ҫʹ�� (index - 1 + capacity) % capacity


    void* target_address = (char*)dq->data + dq->front * dq->element_size;
    memcpy(target_address, element_data, dq->element_size);

    // 4. ���´�С
    dq->size++;
    return true;
}

bool deque_pop_front(Deque* dq, void* output_buffer) {
    /*
     * ��������: �Ӷ�ͷ�Ƴ�Ԫ�� (��ѭ�����е� dequeue ��ȫ��ͬ)��
     * ��ѧ����: capacity=5, size=3, front=1, rear=4
     * ��������: [ | X | A | B | ]
     *              ^           ^
     *            front       rear
     * ����Ҫ pop_front()
    */
    // 1. ���
    if (!dq || !output_buffer || deque_is_empty(dq)) {
        return false;
    }

    // 2. �� front ָ���λ�ÿ�������
    // ����: front=1���� 'X' ������ output_buffer��
    void* source_address = (char*)dq->data + dq->front * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);

    // 3. ���� front ָ�� (��ǰ�ƶ���ѭ��)
    // ����: �� front = (1 + 1) % 5 = 2��front ָ������ָ���� 'A'��
    dq->front = (dq->front + 1) % dq->capacity;

    // 4. ���´�С
    dq->size--;
    return true;
}

bool deque_pop_back(Deque* dq, void* output_buffer) {
    /*
     * ��������: �Ӷ�β�Ƴ�Ԫ�� (����˫�˶��е��¹���)��
     * ��ѧ����: capacity=5, size=2, front=2, rear=4
     * ��������: [ | | A | B |     ]
     *                ^        ^
     *              front    rear
     * ����Ҫ pop_back()
    */
    // 1. ���
    if (!dq || !output_buffer || deque_is_empty(dq)) {
        return false;
    }

    // 2. [�����߼�] ���� rear ָ�� (����ƶ���ѭ��)
    // rear ָ����Ƕ�βԪ�صġ���һ����λ�ã����������Ķ�βԪ���� rear-1��
    // �����Ƚ� rear ָ���ƻض�βԪ�ص�λ�á�
    // ����: rear=4, capacity=5��
    //      �� rear = (4 - 1 + 5) % 5 = 8 % 5 = 3��
    dq->rear = (dq->rear - 1 + dq->capacity) % dq->capacity;

    // 3. ���µ� rear ָ���λ�ÿ�������
    // ����: �� rear=3������βԪ�� 'B' ������ output_buffer��
    void* source_address = (char*)dq->data + dq->rear * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);

    // 4. ���´�С
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
    // �����Ķ�βԪ���� rear ��ǰһ��λ��
    int last_element_index = (dq->rear - 1 + dq->capacity) % dq->capacity;
    void* source_address = (char*)dq->data + last_element_index * dq->element_size;
    memcpy(output_buffer, source_address, dq->element_size);
    return true;
}