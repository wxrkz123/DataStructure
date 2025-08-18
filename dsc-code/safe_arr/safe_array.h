#pragma once
#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool


// 1. safety�� Ҫ���ṩһ��ӿڣ�ȷ���κζ�����ķ��ʶ�����Ϸ���Χ�ڡ�
// 2. encapsulation�� ��װ�ԡ������ڲ���ԭʼ���飬Ҫ���û�����������Ա��ֻ��ͨ����Щ
// �ӿڷ������飬������ֱ�ӷ����ڲ����ݡ�
// 3. usability ������ ��¶���û���API��������ֱ�ۡ�
// interface & information hiding
// --- Opaque Pointer Declaration ��͸��ָ�� ---
// ����ȫ���顱��ħ�����û�ֻ֪���� SafeArray ������ͣ�
// ����ȫ��֪�����ڲ�����ʲôʵ�ֵġ������޷�ֱ�ӷ����ڲ���ԭʼ���顣
// Forward Declaration of SafeArray ǰ������
// ������������� SafeArray ��һ���ṹ�����ͣ����������������ݡ�
// �������ĺô��ǣ��û��޷�ֱ�ӷ��� SafeArray ���ڲ����ݣ�������ǿ��Զ���ָ������ָ�� SafeArray*
typedef struct SafeArray SafeArray;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µİ�ȫ���顣
 *
 * @param capacity ����������ɵ�Ԫ��������
 * @return �ɹ�ʱ����ָ���°�ȫ�����ָ�룬ʧ�ܷ��� NULL��
 */
SafeArray* sarray_create(size_t capacity);

/**
 * @brief ����һ����ȫ���鲢�ͷ������ڴ档
 *
 * @param p_sarray ָ��ȫ����ָ���ָ�룬�����Ὣ����Ϊ NULL��
 */
void sarray_destroy(SafeArray** p_sarray);

/**
 * @brief ��ȫ������ָ������λ�õ�ֵ��
 *
 * @param sarray ָ��Ҫ�����İ�ȫ���顣
 * @param index  Ҫ���õ�Ԫ�ص�������
 * @param value  Ҫ���õ�����ֵ��
 * @return �ɹ����� true���������Խ�������ΪNULL�򷵻� false��
 */
bool sarray_set(SafeArray* sarray, size_t index, int value);

/**
 * @brief ��ȫ�ػ�ȡָ������λ�õ�ֵ��
 *
 * @param sarray ָ��Ҫ�����İ�ȫ���顣
 * @param index  Ҫ��ȡ��Ԫ�ص�������
 * @param out_value ָ��һ��������ָ�룬���ڽ��ջ�ȡ����ֵ��out-parameter �������
 * @return �ɹ����� true���������Խ��������Ч�򷵻� false��
 */
bool sarray_get(const SafeArray* sarray, size_t index, int* out_value);

/**
 * @brief ��ȡ��ȫ�����������
 *
 * @param sarray ָ��Ҫ��ѯ�����顣
 * @return ����������������������ΪNULL�򷵻�0��
 */
size_t sarray_get_capacity(const SafeArray* sarray);

/**
 * @brief (��ѧ��) ��ӡ��ȫ��������ݡ�
 */
void sarray_print(const SafeArray* sarray);