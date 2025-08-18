// main.c

#include "bst.h"
#include <stdio.h>


/*

 * 二叉搜索树 (BST) 模块构建

├── Ⅰ. 奠定基石：设计蓝图与公共接口 (./bst.h)
│   │
│   ├── 1. 定义“客户”的工具箱：函数指针类型
│   │   ├── a. CompareFunc  (讲解：为什么需要它？它是BST的“灵魂”，定义了大小关系)
│   │   └── b. VisitFunc    (讲解：这是为了遍历时能对每个节点做操作的“钩子”)
│   │
│   └── 2. 设计“不透明”的门面：结构体与API原型
│       ├── a. typedef struct BST BST; (讲解：信息隐藏的魔法，用户只知其名不知其形)
│       └── b. 公共API函数原型 (逐一讲解每个函数的作用，这是我们与用户的“合同”)
│           ├── bst_create()
│           ├── bst_destroy()
│           ├── bst_insert()
│           ├── bst_remove()
│           ├── bst_search()
│           └── bst_traverse()
│
├── Ⅱ. 搭建骨架：实现核心结构与简单公共函数 (./bst.c)
│   │
│   ├── 1. 揭秘内部结构：定义私有结构体
│   │   ├── a. Node          (讲解：这是树的基本砖块)
│   │   └── b. BST (struct)  (讲解：这是树的“大脑”，管理着一切)
│   │
│   ├── 2. 实现“构造”与“析构”
│   │   ├── a. bst_create()  (讲解：分配内存，初始化“大脑”)
│   │   └── b. bst_destroy() (讲解：作为CEO，它发起销毁任务，但具体工作要“委托”给下属)
│   │       └── (引出对 _destroy_recursive 的需求)
│   │
│   └── 3. 实现“简单”的公共操作
│       ├── a. bst_search()  (讲解：这是最简单的查找，用迭代实现，逻辑直观)
│       ├── b. bst_is_empty(), bst_get_size() (讲解：这些是简单的状态查询)
│       └── c. bst_traverse() (讲解：作为CEO，它发起遍历任务，具体工作“委托”给下属)
│           └── (引出对 _traverse_recursive 的需求)
│
└── Ⅲ. 填充血肉：实现复杂的私有递归逻辑 (./bst.c 的核心)
    │
    ├── 1. 实现“插入”的递归逻辑 (CEO -> 经理)
    │   ├── a. bst_insert() (复习：回顾CEO的职责)
    │   └── b. _insert_recursive() (深入讲解：部门经理如何递归地找到位置)
    │       ├── b.1. 基准情况 (Base Case): 找到空位，创建节点
    │       └── b.2. 递归步骤: 比较大小，决定向左走还是向右走
    │
    ├── 2. 实现“删除”的递归逻辑 (最难的部分，分步攻克)
    │   ├── a. bst_remove() (复习：回顾CEO的职责)
    │   └── b. _remove_recursive() (深入讲解：这位经理的任务最复杂)
    │       ├── b.1. 查找阶段: 递归地找到要删除的节点
    │       ├── b.2. 删除阶段 - 情况1: 叶子节点 (最简单)
    │       ├── b.3. 删除阶段 - 情况2: 只有一个子节点的节点
    │       └── b.4. 删除阶段 - 情况3: 有两个子节点的节点
    │           └── (此处需要先讲解 _find_min_recursive 辅助函数)
    │
    └── 3. 实现“遍历”与“销毁”的递归逻辑 (作为递归概念的巩固)
        ├── a. _traverse_recursive() (深入讲解：前、中、后序遍历的递归实现)
        └── b. _destroy_recursive() (深入讲解：后序遍历在销毁树时的妙用)
 */

// --- 用户定义的比较和访问函数 ---
// 比较两个整数
int compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// 我们假如写一个比较函数，比较类型是Student的id编号排序
// int compare_student_by_id(const void* a, const void* b) {
//     const Student* student_a = (const Student*)a;
//     const Student* student_b = (const Student*)b;
//     if (student_a->id < student_b->id) return -1;
//     if (student_a->id > student_b->id) return 1;
//	 return 0;
// }

// 打印一个整数
void visit_int(const void* data) {
    printf("%d ", *(const int*)data);
}

void print_in_order(const BST* bst) {
    printf("中序遍历 (有序): ");
    bst_traverse(bst, IN_ORDER, visit_int);
    printf("\n");
}

int main() {
    printf("--- 现代C语言泛型二叉搜索树实现 ---\n");
    BST* bst = bst_create(sizeof(int), compare_int);

    // 1. 插入节点
    printf("\n1. 插入节点: 20, 10, 30, 5, 15, 25, 40, 3, 7\n");
    int values[] = { 20, 10, 30, 5, 15, 25, 40, 3, 7 };
    for (size_t i = 0; i < sizeof(values) / sizeof(int); i++) {
        bst_insert(bst, &values[i]);
    }
    print_in_order(bst);
    printf("当前大小: %zu\n", bst_get_size(bst));

    // 2. 搜索节点
    printf("\n2. 搜索节点...\n");
    int key_to_find = 15;
    printf("搜索 %d: %s\n", key_to_find, bst_search(bst, &key_to_find) ? "找到" : "未找到");
    key_to_find = 99;
    printf("搜索 %d: %s\n", key_to_find, bst_search(bst, &key_to_find) ? "找到" : "未找到");

    // 3. 删除操作 - 情况1: 叶子节点
    printf("\n3. 删除叶子节点 (7)...\n");
    int key_to_remove = 7;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);

    // 4. 删除操作 - 情况2: 只有一个子节点的节点
    // 我们先删除 3，让 5 只有一个右子节点。然后再删除 5。
    printf("\n4. 删除只有一个子节点的节点 (5)...\n");
    key_to_remove = 3;
    bst_remove(bst, &key_to_remove); // 先删掉3
    key_to_remove = 5;
    bst_remove(bst, &key_to_remove); // 再删5，此时5应该只有一个孩子
    print_in_order(bst);

    // 5. 删除操作 - 情况3: 有两个子节点的节点
    printf("\n5. 删除有两个子节点的节点 (10)...\n");
    key_to_remove = 10;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);

    // 6. 删除根节点
    printf("\n6. 删除根节点 (20)...\n");
    key_to_remove = 20;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);
    printf("当前大小: %zu\n", bst_get_size(bst));

    // 7. 销毁树
    printf("\n7. 销毁树...\n");
    bst_destroy(&bst);
    printf("树已销毁，指针为: %s\n", bst == NULL ? "NULL" : "OK");

    return 0;
}