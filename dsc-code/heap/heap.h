#pragma once
#include <stdlib.h> // for size_t

// Ϊ�˴���Ŀɶ��ԺͿ�ά���ԣ�����һ�� Item ����
// �����Ҫ�洢�����������ݣ�ֻ���޸�����
typedef int Item;

// �ѽṹ�嶨��
typedef struct {
    Item* data;         // ָ��洢��Ԫ�صĶ�̬����
    size_t size;        // ���е�ǰ��Ԫ������
    size_t capacity;    // �ѵĵ�ǰ����
} Heap;

/**
 * @brief ����һ��ָ����ʼ����������
 * @param initial_capacity �ѵĳ�ʼ����
 * @return �ɹ��򷵻�ָ��ѵ�ָ�룬ʧ���򷵻� NULL
 */
Heap* heap_create(size_t initial_capacity);

/**
 * @brief ����һ���ѣ��ͷ���������ڴ�
 * @param h ָ��Ҫ���ٵĶѵ�ָ���ָ��
 */
void heap_destroy(Heap** h);

/**
 * @brief ����в���һ����Ԫ��
 * @param h ָ��ѵ�ָ��
 * @param value Ҫ�����ֵ
 * @return �ɹ����� 0��ʧ�ܣ����ڴ����ʧ�ܣ����� -1
 */
int heap_insert(Heap* h, Item value);

/**
 * @brief �Ӷ�����ȡ�����Ƴ������ֵ
 * @param h ָ��ѵ�ָ��
 * @param p_max_value ָ�����ڴ洢���ֵ�ı�����ָ��
 * @return �ɹ���ȡ���� 0�������Ϊ���򷵻� -1
 */
int heap_extract_max(Heap* h, Item* p_max_value);

/**
 * @brief �鿴�Ѷ������ֵ�����Ƴ���
 * @param h ָ��ѵ�ָ��
 * @param p_peek_value ָ�����ڴ洢���ֵ�ı�����ָ��
 * @return �ɹ��鿴���� 0�������Ϊ���򷵻� -1
 */
int heap_peek(const Heap* h, Item* p_peek_value);

/**
 * @brief �����Ƿ�Ϊ��
 * @param h ָ��ѵ�ָ��
 * @return �����Ϊ�շ��� 1�����򷵻� 0
 */
int is_heap_empty(const Heap* h);

/**
 * @brief ��ȡ����Ԫ�ص�����
 * @param h ָ��ѵ�ָ��
 * @return ����Ԫ�ص�����
 */
size_t heap_size(const Heap* h);

/**
 * @brief (��ѡ) ��ӡ�ѵ��ڲ������ʾ����Ҫ���ڵ���
 * @param h ָ��ѵ�ָ��
 */
void heap_print_debug(const Heap* h);