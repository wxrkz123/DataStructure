#define _CRT_SECURE_NO_WARNINGS

#include "Node.h"
#include <stdio.h>
#include <string.h>

// 1. 定义“打印”回调
void print_student(const void* data) {
    const Student* s = (const Student*)data;
    printf("{ID: %d, 姓名: %s, 年龄: %d}", s->id, s->name, s->age);
}

// 2. 定义一个简单的“比较”回调 (只按ID比较)
int compare_by_id(const void* a, const void* b, void* context) {
    // 这个函数不需要额外的上下文，所以 context 参数被忽略
    // (void)context; // 明确表示我们有意不使用这个参数，避免编译器警告

    const Student* s_a = (const Student*)a;
    const Student* target_s = (const Student*)b;
    return s_a->id == target_s->id ? 0 : 1; // 返回0表示相等
}

// 3. 定义一个复杂的“比较”回调 (需要使用上下文)
//    上下文结构体，用于打包额外参数
typedef struct {
    int min_age_required;
} SearchContext;

int compare_by_id_and_min_age(const void* a, const void* b, void* context) {
    const Student* s_a = (const Student*)a;
    const Student* target_s = (const Student*)b;
    SearchContext* ctx = (SearchContext*)context;

    // 复杂的比较逻辑
    if (s_a->id == target_s->id && s_a->age >= ctx->min_age_required) {
        return 0; // 所有条件满足，视为相等
    }
    return 1; // 否则不相等
}

// 4. 定义“释放Data内部资源”的回调 (本例中为空，仅作演示)
void free_student_data(void* data) {
    // 如果 Student 结构体内的 name 是 char* 类型且由 malloc 分配，
    // 此处就需要 free(((Student*)data)->name);
}



int main(void) {
    Node* head = NULL;

    // --- 初始化数据 ---
    printf("--- 1. 初始化链表 ---\n");
    Student students[] = {
        {101, "Alice", 22},
        {102, "Bob", 19},
        {103, "Carol", 25},
        {104, "David", 19}
    };
    for (int i = 0; i < 4; ++i) {
        appendNode(&head, students[i]);
    }
    printList(head, print_student);

    // --- 简单查找与删除 ---
    printf("\n--- 2. 删除学号为103的学生(Carol) ---\n");
    Student target_carol = { 103, "", 0 };
    deleteNode(&head, &target_carol, compare_by_id, NULL); // 简单比较，不需要上下文
    printList(head, print_student);

    // --- 复杂查找与更新 ---
    printf("\n--- 3. 查找学号为104且年龄不小于20岁的学生 ---\n");
    SearchContext ctx_fail = { 20 }; // 设置上下文：最小年龄20
    Student target_david = { 104, "", 0 };
    Node* found = findNode(head, &target_david, compare_by_id_and_min_age, &ctx_fail);
    if (found) {
        printf("找到了！\n");
    }
    else {

        // test
        printf("没找到 (因为David只有19岁)。\n");
    }

    printf("\n--- 4. 查找学号为101且年龄不小于20岁的学生，并更新 ---\n");
    SearchContext ctx_success = { 20 }; // 设置上下文：最小年龄20
    Student target_alice = { 101, "", 0 };
    Student new_alice_data = { 101, "Alicia", 23 };
    updateNode(head, &target_alice, new_alice_data, compare_by_id_and_min_age, &ctx_success);
    printList(head, print_student);

    //// --- 清理 ---
    //printf("\n--- 5. 释放所有内存 ---\n");
    //freeList(&head, free_student_data);
    //printf("链表已清空。\n");
    //printList(head, print_student);

    return 0;
}