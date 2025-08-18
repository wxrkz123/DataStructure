#pragma once

#include <stdbool.h>

typedef struct Trie Trie;

// ����һ������ָ�����ͣ����������û��Զ����ֵ
// ��Trie������ʱ������ڵ��ϴ���ֵ��Trie���������������ͷ�ֵ���ڴ�
typedef void (*ValueDestroyer)(void* value);

/**
 * @brief ����һ���µ�Trieʵ����
 * @param destroyer һ������ָ�룬�����ͷŴ洢��Trie�е�ֵ�����ֵΪ������
 *                  �����ⲿ�����ڴ棬���Դ���NULL��
 * @return �ɹ�ʱ����ָ����Trie��ָ�룻ʧ�ܷ���NULL��
 */
Trie* Trie_Create(ValueDestroyer destroyer);

void Trie_Destroy(Trie* trie);

/**
 * @brief ��Trie�в���һ����ֵ�ԡ�
 * @param trie ָ��Trieʵ����ָ�롣
 * @param key Ҫ����ļ������ʣ���
 * @param value ָ��Ҫ�洢��ֵ��ָ�롣���һ�����Ѵ��ڣ����ֵ�������ǡ�
 *              ��ע�⣺�ⲻ���Զ��ͷž�ֵ����Ҫ�û��Լ�������
 * @return �������ɹ�������true�����򷵻�false��
 */
bool Trie_Insert(Trie* trie, const char* key, void* value);

/**
 * @brief ��Trie������һ�������������������ֵ��
 * @param trie ָ��Trieʵ����ָ�롣
 * @param key Ҫ�����ļ���
 * @return ����ҵ��������ع�����ֵ��ָ�룻���򷵻�NULL��
 */
void* Trie_Search(const Trie* trie, const char* key);

bool Trie_StartsWith(const Trie* trie, const char* prefix);

void Trie_Delete(Trie* trie, const char* key);

