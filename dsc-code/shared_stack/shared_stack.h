#pragma once

#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool

// --- Public Enumeration and Opaque Pointer ---

// ʹ��ö���������ر�ʶҪ������ջ
typedef enum {
    STACK_ONE = 1, // ջ1��������ͷ����ʼ
    STACK_TWO = 2  // ջ2��������β����ʼ
} StackNumber;

// ��͸��ָ�룬�û��޷������ڲ��ṹ
typedef struct SharedStack Stack;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µĹ���ջ��
 *
 * @param total_capacity ����ջ�������������Ԫ�ظ�������
 * @param element_size   ÿ��Ԫ�صĴ�С�����ֽ�Ϊ��λ����
 * @return �ɹ�ʱ����ָ����ջ��ָ�룬ʧ�ܷ��� NULL��
 */
Stack* stack_create(size_t total_capacity, size_t element_size);

/**
 * @brief ����һ������ջ���ͷ������ڴ档
 *
 * @param p_stack ָ��ջָ���ָ�룬�����Ὣ����Ϊ NULL��
 */
void stack_destroy(Stack** p_stack);

/**
 * @brief ��һ��Ԫ��ѹ��ָ����ջ��
 *
 * @param stack ָ��Ҫ������ջ��
 * @param num   Ҫ������ջ��� (STACK_ONE �� STACK_TWO)��
 * @param element_data ָ��Ҫѹ���Ԫ�����ݵ�ָ�롣
 * @return �ɹ����� true�����ջ���������Ч�򷵻� false��
 */
bool stack_push(Stack* stack, StackNumber num, const void* element_data);

/**
 * @brief ��ָ����ջ����һ��Ԫ�ء�
 *
 * @param stack ָ��Ҫ������ջ��
 * @param num   Ҫ������ջ��� (STACK_ONE �� STACK_TWO)��
 * @param output_buffer ָ�����ڽ��յ���Ԫ�����ݵĻ�������
 * @return �ɹ����� true�����ջ�ջ������Ч�򷵻� false��
 */
bool stack_pop(Stack* stack, StackNumber num, void* output_buffer);

/**
 * @brief �鿴ָ��ջ��ջ��Ԫ�أ�����������
 *
 * @param stack ָ��Ҫ������ջ��
 * @param num   Ҫ������ջ��� (STACK_ONE �� STACK_TWO)��
 * @param output_buffer ָ�����ڽ���ջ��Ԫ�����ݵĻ�������
 * @return �ɹ����� true�����ջ�ջ������Ч�򷵻� false��
 */
bool stack_peek(const Stack* stack, StackNumber num, void* output_buffer);

/**
 * @brief ��鹲��ռ��Ƿ�������
 *
 * @param stack ָ��Ҫ����ջ��
 * @return ����������� true�����򷵻� false��
 */
bool stack_is_full(const Stack* stack);

/**
 * @brief ���ָ����ջ�Ƿ�Ϊ�ա�
 *
 * @param stack ָ��Ҫ����ջ��
 * @param num   Ҫ����ջ��� (STACK_ONE �� STACK_TWO)��
 * @return �����ջΪ�շ��� true�����򷵻� false��
 */
bool stack_is_empty(const Stack* stack, StackNumber num);

/**
 * @brief ��ȡָ��ջ�ĵ�ǰԪ��������
 *
 * @param stack ָ��Ҫ������ջ��
 * @param num   Ҫ��ȡ��С��ջ��� (STACK_ONE �� STACK_TWO)��
 * @return ���ظ�ջ��Ԫ��������
 */
size_t stack_get_size(const Stack* stack, StackNumber num);

/**
 * @brief ��ȡ����ջ����������
 *
 * @param stack ָ��Ҫ������ջ��
 * @return ������������
 */
size_t stack_get_total_capacity(const Stack* stack);
