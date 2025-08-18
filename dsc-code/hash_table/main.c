// main.c

#include "hash_table.h"
#include <stdio.h>

int main() {
    printf("--- �ִ���ϣ�� C����ʵ�� (������) ---\n");
    HashTable* ht = ht_create(10); // ��������Ϊ10�Ĺ�ϣ��
    if (!ht) {
        printf("������ϣ��ʧ�ܡ�\n");
        return 1;
    }

    printf("\n1. �����ʼ���� (������ϣ��ͻ)...\n");
    // hash("name") % 10 = (110+97+109+101)%10 = 417%10 = 7 (ע��ASCII�벻ͬ��������΢��������ͻԭ��һ��)
    // hash("mane") % 10 = (109+97+110+101)%10 = 417%10 = 7 
    ht_set(ht, "name", "Alice");
    ht_set(ht, "age", "30");
    ht_set(ht, "city", "New York");
    ht_set(ht, "mane", "Bob"); // <-- ��������� "name" ������ϣ��ͻ
    ht_print(ht);

    printf("2. ��������...\n");
    const char* name = ht_get(ht, "name");
    const char* city = ht_get(ht, "city");
    const char* country = ht_get(ht, "country"); // һ�������ڵļ�
    printf("Get 'name': %s\n", name ? name : "Not Found");
    printf("Get 'city': %s\n", city ? city : "Not Found");
    printf("Get 'country': %s\n", country ? country : "Not Found");

    // ���ҳ�ͻ���е���һ����
    const char* mane = ht_get(ht, "mane");
    printf("Get 'mane' (collision key): %s\n", mane ? mane : "Not Found");
    printf("\n");

    printf("3. ��������...\n");
    printf("Updating 'name' from 'Alice' to 'Amy'...\n");
    ht_set(ht, "name", "Amy");
    ht_print(ht);

    printf("4. ɾ������...\n");
    printf("Removing 'age'...\n");
    ht_remove(ht, "age");
    ht_print(ht);

    printf("Removing 'name' (a key in a collision chain)...\n");
    ht_remove(ht, "name");
    ht_print(ht);

    printf("5. ���ٹ�ϣ��...\n");
    ht_destroy(&ht);
    printf("��ϣ�������٣�ָ��Ϊ: %s\n", ht == NULL ? "NULL" : "OK");

    return 0;
}