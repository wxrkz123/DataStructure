#pragma once
#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool (C99/C11 standard)

// --- Opaque Pointer Declaration ---
// �û�ֻ֪���� Stack ������ͣ�����֪�����ڲ������������顣
typedef struct LinkedStack Stack;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µ�ͨ����ʽջ��
 *
 * @param element_size ջ��ÿ��Ԫ�صĴ�С�����ֽ�Ϊ��λ�������� sizeof(int)��
 * @return �ɹ�ʱ����ָ����ջ��ָ�룬��� element_size Ϊ0���ڴ����ʧ�ܣ��򷵻� NULL��
 */
Stack* stack_create(size_t element_size);

/**
 * @brief ����һ��ջ���ͷ������нڵ������ڴ档
 *
 * @param p_stack ָ��ջָ���ָ�롣����ִ�к�*p_stack ��������Ϊ NULL��
 */
void stack_destroy(Stack** p_stack);

/**
 * @brief ��һ��Ԫ��ѹ��ջ����
 *
 * @param stack ָ��Ҫ������ջ��ָ�롣
 * @param element_data ָ��Ҫѹ��ջ��Ԫ�����ݵ�ָ�롣
 * @return �ɹ�ѹջ���� true������ڴ����ʧ�ܻ������Ч�򷵻� false��
 */
bool stack_push(Stack* stack, const void* element_data);

/**
 * @brief ��ջ������һ��Ԫ�ء�
 *
 * @param stack ָ��Ҫ������ջ��ָ�롣
 * @param output_buffer ָ��һ����������ָ�룬���ڽ��յ�����Ԫ�����ݡ�
 * @return �ɹ���ջ���� true�����ջΪ�ջ������Ч�򷵻� false��
 */
bool stack_pop(Stack* stack, void* output_buffer);

/**
 * @brief �鿴ջ��Ԫ�أ��������䵯����
 *
 * @param stack ָ��Ҫ������ջ��ָ�롣
 * @param output_buffer ָ��һ����������ָ�룬���ڽ���ջ��Ԫ�ص����ݡ�
 * @return �ɹ��鿴���� true�����ջΪ�ջ������Ч�򷵻� false��
 */
bool stack_peek(const Stack* stack, void* output_buffer);

/**
 * @brief ���ջ�Ƿ�Ϊ�ա�
 *
 * @param stack ָ��Ҫ������ջ��ָ�롣
 * @return ���ջΪ�ջ�ΪNULL�򷵻� true�����򷵻� false��
 */
bool stack_is_empty(const Stack* stack);

/**
 * @brief ��ȡջ�е�ǰ��Ԫ��������
 *
 * @param stack ָ��Ҫ������ջ��ָ�롣
 * @return ����ջ�е�Ԫ�����������ջΪNULL������0��
 */
size_t stack_get_size(const Stack* stack);

