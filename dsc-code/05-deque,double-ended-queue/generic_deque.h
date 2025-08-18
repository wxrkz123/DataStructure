#pragma once
#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Declaration ---
typedef struct Deque Deque;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µķ���˫�˶��У�����ѭ�����飩��
 *
 * @param capacity ���е����������
 * @param element_size ÿ��Ԫ�صĴ�С���ֽڣ���
 * @return �ɹ�ʱ����ָ���¶��е�ָ�룬ʧ�ܷ��� NULL��
 */
Deque* deque_create(size_t capacity, size_t element_size);

/**
 * @brief ����˫�˶��в��ͷ������ڴ档
 */
void deque_destroy(Deque** p_deque);

/**
 * @brief �ڶ�ͷ���һ��Ԫ�ء�
 */
bool deque_push_front(Deque* dq, const void* element_data);

/**
 * @brief �ڶ�β���һ��Ԫ�ء�
 */
bool deque_push_back(Deque* dq, const void* element_data);

/**
 * @brief �Ӷ�ͷ�Ƴ�һ��Ԫ�ء�
 */
bool deque_pop_front(Deque* dq, void* output_buffer);

/**
 * @brief �Ӷ�β�Ƴ�һ��Ԫ�ء�
 */
bool deque_pop_back(Deque* dq, void* output_buffer);

/**
 * @brief �鿴��ͷԪ�ء�
 */
bool deque_peek_front(const Deque* dq, void* output_buffer);

/**
 * @brief �鿴��βԪ�ء�
 */
bool deque_peek_back(const Deque* dq, void* output_buffer);

bool deque_is_empty(const Deque* dq);
bool deque_is_full(const Deque* dq);
size_t deque_get_size(const Deque* dq);