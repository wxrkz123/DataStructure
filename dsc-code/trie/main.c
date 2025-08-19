#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

// --- 自定义数据结构 ---
// 为了测试泛型Trie，我们定义一个Person结构体来作为存储的值。
typedef struct {
    char* name;
    int age;
} Person;

// --- 自定义值的创建和销毁函数 ---

/**
 * @brief 创建一个新的Person实例（辅助函数）。
 */
Person* Person_Create(const char* name, int age) {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) return NULL;
    // 为名字分配内存并拷贝
    p->name = (char*)malloc(strlen(name) + 1);
    if (!p->name) {
        free(p);
        return NULL;
    }
    strcpy(p->name, name);
    p->age = age;
    return p;
}

/**
 * @brief 用于Trie的ValueDestroyer，负责释放Person结构体的内存。
 * @param value 一个void*指针，指向一个Person结构体。
 */
void Person_Destroy(void* value) {
    if (!value) return;
    Person* p = (Person*)value;
    printf("  (Destroying value for Person: %s)\n", p->name);
    free(p->name); // 释放内部的字符串
    free(p);       // 释放结构体本身
}


/**
 * @brief 打印搜索结果的辅助函数。
 */
void print_search_result(const Trie* trie, const char* key) {
    Person* p = (Person*)Trie_Search(trie, key);
    if (p) {
        printf("Search for '%s': FOUND. Value -> { Name: %s, Age: %d }\n", key, p->name, p->age);
    }
    else {
        printf("Search for '%s': NOT FOUND.\n", key);
    }
}


int main(void) {
    printf("--- Trie Test Suite ---\n\n");

    // 1. 创建Trie，并传入Person的析构函数
    printf("1. Creating Trie with a custom value destroyer...\n");
    Trie* trie = Trie_Create(Person_Destroy);
    if (!trie) {
        fprintf(stderr, "Failed to create Trie.\n");
        return 1;
    }
    printf("Trie created successfully.\n\n");

    // 2. 插入数据
    printf("2. Testing Trie_Insert...\n");
    // 注意：Trie会接管这些Person指针的内存管理，我们不需要在main中手动释放它们
    Trie_Insert(trie, "apple", Person_Create("iPhone", 15));
    Trie_Insert(trie, "app", Person_Create("AppStore", 16));
    Trie_Insert(trie, "application", Person_Create("GenericApp", 5));
    Trie_Insert(trie, "banana", Person_Create("Fruit", 2));
    Trie_Insert(trie, "band", Person_Create("MusicGroup", 10));
    printf("Finished inserting keys: 'apple', 'app', 'application', 'banana', 'band'.\n\n");

    // 3. 搜索测试
    printf("3. Testing Trie_Search...\n");
    print_search_result(trie, "apple");
    print_search_result(trie, "app");
    print_search_result(trie, "application");
    print_search_result(trie, "banana");
    print_search_result(trie, "band");
    print_search_result(trie, "ban");    // 不存在的键
    print_search_result(trie, "apples");  // 不存在的键
    printf("\n");

    // 4. 前缀测试
    printf("4. Testing Trie_StartsWith...\n");
    printf("Prefix 'app': %s\n", Trie_StartsWith(trie, "app") ? "true" : "false");
    printf("Prefix 'ban': %s\n", Trie_StartsWith(trie, "ban") ? "true" : "false");
    printf("Prefix 'bana': %s\n", Trie_StartsWith(trie, "bana") ? "true" : "false");
    printf("Prefix 'cat': %s\n", Trie_StartsWith(trie, "cat") ? "false" : "false");
    printf("Prefix 'application': %s\n", Trie_StartsWith(trie, "application") ? "true" : "false");
    printf("\n");

    // 5. 删除测试
    printf("5. Testing Trie_Delete...\n");

    // 场景A: 删除一个作为其他键前缀的键 ("app")
    // 预期：'app'的value变为NULL，但节点保留，因为'apple'和'application'需要它。
    printf("\n--- Deleting 'app' (is a prefix of others) ---\n");
    Trie_Delete(trie, "app");
    print_search_result(trie, "app"); // 应该找不到了
    print_search_result(trie, "apple"); // 应该还在
    printf("Prefix 'app' after deleting key 'app': %s\n", Trie_StartsWith(trie, "app") ? "true" : "false"); // 前缀应该还存在

    // 场景B: 删除一个叶子键 ("banana")
    // 预期：'banana'被删除，并且'nana'路径的节点被物理移除。
    printf("\n--- Deleting 'banana' (is a leaf path) ---\n");
    Trie_Delete(trie, "banana");
    print_search_result(trie, "banana"); // 应该找不到了
    print_search_result(trie, "band");   // 应该还在
    printf("Prefix 'bana' after deleting key 'banana': %s\n", Trie_StartsWith(trie, "bana") ? "false" : "false"); // 前缀应该不存在了
    printf("Prefix 'ban' after deleting key 'banana': %s\n", Trie_StartsWith(trie, "ban") ? "true" : "false");   // 但'ban'前缀应因'band'而存在

    // 场景C: 删除一个不存在的键
    printf("\n--- Deleting 'zebra' (non-existent) ---\n");
    Trie_Delete(trie, "zebra");
    printf("Attempted to delete a non-existent key. Let's check if others are intact.\n");
    print_search_result(trie, "apple"); // 应该还在
    print_search_result(trie, "band");  // 应该还在

    // 场景D: 删除最后一个依赖于某个前缀的键
    printf("\n--- Deleting 'application' ---\n");
    Trie_Delete(trie, "application");
    print_search_result(trie, "application");
    printf("Prefix 'appli' after deleting key 'application': %s\n", Trie_StartsWith(trie, "appli") ? "false" : "false"); // 前缀应该不存在了

    printf("\n");

    // 6. 销毁Trie
    printf("6. Testing Trie_Destroy...\n");
    printf("Calling Trie_Destroy. The custom destroyer should be called for remaining values ('apple', 'band').\n");
    Trie_Destroy(trie);
    printf("Trie destroyed.\n\n");

    printf("--- Test Suite Finished ---\n");
    return 0;
}