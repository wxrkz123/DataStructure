// main.c
// #define _CRT_SECURE_NO_WARNINGS // 如果在 VS2022 中使用 strcpy 等函数需要这个
#include <stdio.h>
#include <stdlib.h>
#include "DoublyLinkedList.h"

int main() {
    printf("=== Doubly Linked List Music Player Demo ===\n\n");

    // 1. 创建一个空的播放列表
    printf(">> Initializing an empty playlist...\n");
    DoublyLinkedList* playlist = createList();
    if (!playlist) {
        return 1; // 内存分配失败，退出
    }
    printListForward(playlist);
    printf("\n");

    // 2. 添加初始歌曲 (Append)
    printf(">> Adding initial songs to the playlist using append()...\n");
    append(playlist, (Song) { "As It Was", "Harry Styles", 167 });
    append(playlist, (Song) { "Levitating", "Dua Lipa", 203 });
    append(playlist, (Song) { "good 4 u", "Olivia Rodrigo", 178 });
    append(playlist, (Song) { "Blinding Lights", "The Weeknd", 200 });
    printListForward(playlist);
    printf("\n");

    // 3. 演示向前和向后遍历
    printf(">> Verifying backward traversal...\n");
    printListBackward(playlist);
    printf("\n");

    // 4. 演示插入操作 (Insert After)
    printf(">> Action: Inserting 'Industry Baby' after 'Levitating'.\n");
    printf("   This is an O(1) operation once the target node is found.\n");
    Node* levitatingNode = findByTitle(playlist, "Levitating");
    if (levitatingNode) {
        insertAfter(playlist, levitatingNode, (Song) { "Industry Baby", "Lil Nas X", 212 });
        printListForward(playlist);
    }
    else {
        printf("   Could not find 'Levitating'.\n");
    }
    printf("\n");

    // 5. 演示在头部插入 (Prepend)
    printf(">> Action: Inserting 'First Class' at the very beginning.\n");
    printf("   This is an O(1) operation.\n");
    prepend(playlist, (Song) { "First Class", "Jack Harlow", 174 });
    printListForward(playlist);
    printf("\n");

    // 6. 演示删除操作 (Delete)
    printf(">> Action: Deleting 'good 4 u' from the playlist.\n");
    printf("   This is an O(1) operation once the target node is found.\n");
    Node* good4uNode = findByTitle(playlist, "good 4 u");
    if (good4uNode) {
        deleteNode(playlist, good4uNode);
        printListForward(playlist);
    }
    else {
        printf("   Could not find 'good 4 u'.\n");
    }
    printf("\n");

    // 7. 演示删除头节点
    printf(">> Action: Deleting the head node ('First Class').\n");
    Node* headNode = playlist->head;
    if (headNode) {
        deleteNode(playlist, headNode);
        printListForward(playlist);
    }
    printf("\n");

    // 8. 演示删除尾节点
    printf(">> Action: Deleting the tail node ('Blinding Lights').\n");
    Node* tailNode = playlist->tail;
    if (tailNode) {
        deleteNode(playlist, tailNode);
        printListForward(playlist);
    }
    printf("\n");

    // 9. 清理工作
    printf(">> Demo finished. Freeing all allocated memory...\n");
    freeList(playlist);
    printf("   Memory has been successfully freed.\n");

    return 0;
}