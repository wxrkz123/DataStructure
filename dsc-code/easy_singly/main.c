#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义节点结构
// 1. data int
// 2. 指向下一个节点的指针
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNote(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("错误：分配失败!\n");
        return NULL;
    } 

    newNode->data = data;
    newNode->next = NULL;


    return newNode;
}

// headRef是指向head指针的指针
// 这样我们才能够在链表为空的时候，修改head
void list_append(Node** headRef, int data) {
    Node* newNode = createNote(data);
    if (newNode == NULL) return;

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    // head 是你家的一张便签纸，上面写着你好朋友家的地址
    // head 一级指针 Node* 就是是便签纸
    // 链表数据：你朋友家，以及他家的连着他家户口本...
    // Node* list_append 跑腿小哥

    // head NULL

    Node* last = *headRef;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newNode;

}


void prependNode(Node** headRef, int data) {
    Node* newNode = createNote(data);
    
    newNode->next = *headRef;
    *headRef = newNode;

}


void printList(Node* head) {
    Node* current = head;

    printf("当前列表:");
    while (current != NULL) {
        printf("%d => ", current->data);
        current = current->next;
    }

    printf("NULL\n");

}

Node* findNode(Node* head, int data) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void updateNode(Node* head, int oldData, int newData) {
    Node* nodeToUpdate = findNode(head, oldData);
    if (nodeToUpdate != NULL) {
        nodeToUpdate->data = newData;
        printf("数据 %d 已经成功更新至 %d .\n", oldData, newData);
    }
    else {
        printf("未找到!\n");
    }
}

void deleteNode(Node** headRef, int data) {
    Node* temp = *headRef;
    Node* prev = NULL;

    // 如果删除的是头节点
    if (temp != NULL && temp->data == data) {
        *headRef = temp->next;
        free(temp);
        return;
    }

    // head --> [10 | next] --> [20 | next] --> [30 | next] --> NULL
    //              head
    //                                              temp
    //                              prev
    // 如果是在尾部或者中间
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void list_free(Node** headRef) {
    if (headRef == NULL) return;
    Node* current = *headRef;

    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    *headRef = NULL;
}

int main(void) {
   
    Node* head = NULL;

    list_append(&head, 10);
    list_append(&head, 20);
    list_append(&head, 30);
   
    printList(head);


    list_append(&head, 40);
    printList(head);


    prependNode(&head, 5);
    printList(head);

    list_free(&head);

    printList(head);


    return 0;
}