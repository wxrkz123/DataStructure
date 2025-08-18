#pragma once

#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Declaration ---
// �û�ֻ֪���� Queue ������ͣ���֪�����ڲ������������顣
typedef struct LinkedQueue Queue;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µķ�����ʽ���С�
 *
 * @param element_size ÿ��Ԫ�صĴ�С�����ֽ�Ϊ��λ����
 * @return �ɹ�ʱ����ָ���¶��е�ָ�룬ʧ�ܷ��� NULL��
 */
Queue* queue_create(size_t element_size);

/**
 * @brief ����һ�����в��ͷ������нڵ������ڴ档
 *
 * @param p_queue ָ�����ָ���ָ�룬�����Ὣ����Ϊ NULL��
 */
void queue_destroy(Queue** p_queue);

/**
 * @brief ��һ��Ԫ����ӵ���β����ӣ���
 *
 * @param queue ָ��Ҫ�����Ķ��С�
 * @param element_data ָ��Ҫ��ӵ�Ԫ�����ݵ�ָ�롣
 * @return �ɹ����� true������ڴ����ʧ�ܻ������Ч�򷵻� false��
 */
bool queue_enqueue(Queue* queue, const void* element_data);

/**
 * @brief �Ӷ�ͷ�Ƴ�һ��Ԫ�أ����ӣ���
 *
 * @param queue ָ��Ҫ�����Ķ��С�
 * @param output_buffer ָ�����ڽ��ճ���Ԫ�����ݵĻ�������
 * @return �ɹ����� true���������Ϊ�ջ������Ч�򷵻� false��
 */
bool queue_dequeue(Queue* queue, void* output_buffer);

/**
 * @brief �鿴��ͷԪ�أ����������Ƴ���
 *
 * @param queue ָ��Ҫ�����Ķ��С�
 * @param output_buffer ָ�����ڽ��ն�ͷԪ�����ݵĻ�������
 * @return �ɹ����� true���������Ϊ�ջ������Ч�򷵻� false��
 */
bool queue_peek(const Queue* queue, void* output_buffer);

/**
 * @brief �������Ƿ�Ϊ�ա�
 *
 * @param queue ָ��Ҫ���Ķ��С�
 * @return �������Ϊ�շ��� true�����򷵻� false��
 */
bool queue_is_empty(const Queue* queue);

/**
 * @brief ��ȡ�����е�ǰ��Ԫ��������
 *
 * @param queue ָ��Ҫ�����Ķ��С�
 * @return ���ض����е�Ԫ��������
 */
size_t queue_get_size(const Queue* queue);