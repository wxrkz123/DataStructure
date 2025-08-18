#pragma once
#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Declaration ---
// �û�ֻ֪���� HashTable ������ͣ����޷��������ڲ���Ա��ʵ���������ķ�װ��
typedef struct HashTable HashTable;

// --- Public API Prototypes ---

/**
 * @brief ����һ���µĹ�ϣ��
 *
 * @param capacity ��ϣ���������Ͱ����������ѡ��һ�����ʵ�����ͨ���ܻ�ø��õķֲ���
 * @return �ɹ�ʱ����ָ���¹�ϣ���ָ�룬����ڴ����ʧ�ܻ�����Ϊ0�򷵻� NULL��
 */
HashTable* ht_create(size_t capacity);

/**
 * @brief ����һ����ϣ�����ͷ���������ڴ棨�������м���ֵ����
 *
 * @param p_ht ָ���ϣ��ָ���ָ�롣����ִ�к�*p_ht ��������Ϊ NULL��
 */
void ht_destroy(HashTable** p_ht);

/**
 * @brief �ڹ�ϣ���в�������һ����ֵ�ԡ�
 *
 * ������Ѵ��ڣ����Ӧ��ֵ�������¡�
 * ����������ڣ�������һ���µļ�ֵ�Բ����롣
 * ������Ϊ����ֵ�����������ڴ濽�������Ե����߿����ͷ��Լ��İ汾��
 *
 * @param ht ָ��Ҫ�����Ĺ�ϣ��
 * @param key Ҫ����/���µļ���һ��C�ַ�������
 * @param value ������ֵ��һ��C�ַ�������
 * @return �ɹ����� true�����������Ч���ڴ����ʧ���򷵻� false��
 */
bool ht_set(HashTable* ht, const char* key, const char* value);

/**
 * @brief �ӹ�ϣ���в���һ�������������Ӧ��ֵ��
 *
 * @param ht ָ��Ҫ�����Ĺ�ϣ��
 * @param key Ҫ���ҵļ���
 * @return ����ҵ�������һ��ָ��ֵ�ĳ����ַ���ָ�롣
 *         **����**: ���ص�ָ��ָ���ϣ���ڲ����ݣ���Ӧ���޸Ļ��ͷš�
 *         ���δ�ҵ������� NULL��
 */
const char* ht_get(const HashTable* ht, const char* key);

/**
 * @brief �ӹ�ϣ����ɾ��һ����ֵ�ԡ�
 *
 * @param ht ָ��Ҫ�����Ĺ�ϣ��
 * @param key Ҫɾ���ļ���
 * @return ����ҵ����ɹ�ɾ���򷵻� true�����򷵻� false��
 */
bool ht_remove(HashTable* ht, const char* key);

/**
 * @brief (��������) ��ӡ��ϣ����ڲ��ṹ�����ڵ��Ժͽ�ѧ��
 */
void ht_print(const HashTable* ht);