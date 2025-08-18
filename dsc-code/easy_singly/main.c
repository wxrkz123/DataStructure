#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����ڵ�ṹ
// 1. data int
// 2. ָ����һ���ڵ��ָ��
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNote(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("���󣺷���ʧ��!\n");
        return NULL;
    } 

    newNode->data = data;
    newNode->next = NULL;


    return newNode;
}

// headRef��ָ��headָ���ָ��
// �������ǲ��ܹ�������Ϊ�յ�ʱ���޸�head
void list_append(Node** headRef, int data) {
    Node* newNode = createNote(data);
    if (newNode == NULL) return;

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    // head ����ҵ�һ�ű�ǩֽ������д��������Ѽҵĵ�ַ
    // head һ��ָ�� Node* �����Ǳ�ǩֽ
    // �������ݣ������Ѽң��Լ����ҵ��������һ��ڱ�...
    // Node* list_append ����С��

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

    printf("��ǰ�б�:");
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
        printf("���� %d �Ѿ��ɹ������� %d .\n", oldData, newData);
    }
    else {
        printf("δ�ҵ�!\n");
    }
}

void deleteNode(Node** headRef, int data) {
    Node* temp = *headRef;
    Node* prev = NULL;

    // ���ɾ������ͷ�ڵ�
    if (temp != NULL && temp->data == data) {
        *headRef = temp->next;
        free(temp);
        return;
    }

    // head --> [10 | next] --> [20 | next] --> [30 | next] --> NULL
    //              head
    //                                              temp
    //                              prev
    // �������β�������м�
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