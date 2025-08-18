#pragma once

#include <stdbool.h>

typedef struct Trie Trie;

// 定义一个函数指针类型，用于销毁用户自定义的值
// 当Trie被销毁时，如果节点上存有值，Trie会调用这个函数来释放值的内存
typedef void (*ValueDestroyer)(void* value);

/**
 * @brief 创建一个新的Trie实例。
 * @param destroyer 一个函数指针，用于释放存储在Trie中的值。如果值为简单类型
 *                  或由外部管理内存，可以传入NULL。
 * @return 成功时返回指向新Trie的指针；失败返回NULL。
 */
Trie* Trie_Create(ValueDestroyer destroyer);

void Trie_Destroy(Trie* trie);

/**
 * @brief 向Trie中插入一个键值对。
 * @param trie 指向Trie实例的指针。
 * @param key 要插入的键（单词）。
 * @param value 指向要存储的值的指针。如果一个键已存在，其旧值将被覆盖。
 *              （注意：库不会自动释放旧值，需要用户自己管理）。
 * @return 如果插入成功，返回true；否则返回false。
 */
bool Trie_Insert(Trie* trie, const char* key, void* value);

/**
 * @brief 在Trie中搜索一个键，并返回其关联的值。
 * @param trie 指向Trie实例的指针。
 * @param key 要搜索的键。
 * @return 如果找到键，返回关联的值的指针；否则返回NULL。
 */
void* Trie_Search(const Trie* trie, const char* key);

bool Trie_StartsWith(const Trie* trie, const char* prefix);

void Trie_Delete(Trie* trie, const char* key);

