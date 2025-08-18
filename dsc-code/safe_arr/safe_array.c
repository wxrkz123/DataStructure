// safe_array.c

#include "safe_array.h"
#include <stdio.h>
#include <stdlib.h>

// --- Private Structure Definition ---
// ����ȫ���顱���ڲ����ܣ�����ʵ��һ���ṹ�壬
// ��װ��������ԭʼ����ָ�������������
struct SafeArray {
    int* data;      // ָ��̬�����ԭʼ��������
    size_t capacity; // ���������
};

// ======================================================================
// ====================  ˽�и������� - �����ž���  ====================
// ======================================================================
// ��Щ����������Ϊ static����ζ������ֻ���ڱ��ļ�(.c)�ڲ������á�
// �����ǡ��ڲ�Ա�������û�������.c�ļ�����Զ�޷�ֱ�ӽӴ������ǡ�
// ���Ǽ������м�鹤���Ѿ����ϼ������ˣ�ֻרע���Լ��ĺ�������

/**
 * @brief [˽��] ����ִ������ֵ�Ĳ�����
 * @note  ��������ǡ�����ȫ���ģ���Ϊ��������������Ч�ġ�
 *        ���Ĵ��ڣ���Ϊ�˰ѡ����Ĳ������͡���ȫ��顱���뿪��
 */
static void _set_value(SafeArray* sarray, size_t index, int value) {
    // ���ž���ĺ��Ĺ���������һ�д��룡
    // ����������CEO�ϼ��Ѿ�����index�ĺϷ����ˡ�
    sarray->data[index] = value;
}

/**
 * @brief [˽��] ����ִ�л�ȡֵ�Ĳ�����
 */
static int _get_value(const SafeArray* sarray, size_t index) {
    // ���ž���ĺ��Ĺ�����Ҳֻ����һ�У�
    return sarray->data[index];
}

// ======================================================================
// ====================  ����API���� - ����˾CEO��  ======================
// ======================================================================

SafeArray* sarray_create(size_t capacity) {
    // CEO��ְ��1������ʼ�����ĺϷ���
    if (capacity == 0) return NULL;

    SafeArray* sa = (SafeArray*)malloc(sizeof(SafeArray));
    if (!sa) return NULL;

    // CEO��ְ��2��������Դ�ķ���
    sa->data = (int*)malloc(capacity * sizeof(int));
    if (!sa->data) {
        free(sa);
        return NULL;
    }

    sa->capacity = capacity;
    return sa;
}

void sarray_destroy(SafeArray** p_sarray) {
    // CEO��ְ�𣺸�����Դ���ͷ�
    if (p_sarray && *p_sarray) {
        free((*p_sarray)->data); // ���ͷ��ڲ�������
        free(*p_sarray);         // ���ͷŽṹ�屾��
        *p_sarray = NULL;
    }
}

bool sarray_set(SafeArray* sarray, size_t index, int value) {
    /*
     * ��������: [CEO] ��ȫ������һ��ֵ��
     * ------------------------- ��ѧ���� -------------------------
     * �������ǵ��� sarray_set(my_array, 2, 99)��
     * 1. CEO (sarray_set) �ӵ�����
     * 2. CEO ���ĵ�һ���£����ǽ����ϸ�ġ����족��
     *    - ��� my_array �Ƿ���ڣ�(sarray != NULL)
     *    - ������� 2 �Ƿ��ںϷ���Χ�� [0, capacity-1]��
     * 3. �������ͨ����CEO �Ͱѡ���99���ֵ�ŵ�2��λ�á�������������
     *    ί�и��ڲ��ġ����ž��� `_set_value` ȥִ�С�
     * 4. CEO �����ľ����������ôд�ģ���ֻ֪���������ɡ�
     * 5. CEO ��ͻ����棺����ɹ� (return true)��
     * -----------------------------------------------------------
    */

    // ���� 1: CEO�����ϸ�ġ����족�����ǹ���API�ĺ���ְ��
    // �������ָ���Ƿ���Ч���Լ������Ƿ�Խ�硣
    if (!sarray || index >= sarray->capacity) {
        // ������첻ͨ����ֱ�Ӿܾ����񣬱���ʧ�ܡ�
        return false;
    }

    // ���� 2: ����ͨ���󣬽���������ί�и������ž�����
    // 
    // 
    // ����3�� ����ί�С�����������ְ����룺CEO����ȫ��������ִ�С�
    _set_value(sarray, index, value);


    // 
    // 
    //    sarray->data[index] = value;

    // ְ�����
	// Separation of Concerns


    // ���� 3: ��������ɹ���
    return true;
}

bool sarray_get(const SafeArray* sarray, size_t index, int* out_value) {
    /*
     * ��������: [CEO] ��ȫ�ػ�ȡһ��ֵ��
     * �߼��� sarray_set ��ȫ��ͬ��ֻ��ί�и��˲�ͬ�ġ����ž�����
    */

    // ���� 1: CEO�����ϸ�ġ����족��
    // �������ָ�롢�������Լ����ڽ��ս�������ָ���Ƿ���Ч��
    if (!sarray || index >= sarray->capacity || !out_value) {
        return false;
    }

    // ���� 2: ����ͨ����ί�и������ž���`_get_value`ȥȡ�����ݡ�
    int value = _get_value(sarray, index);

    // ���� 3: ������ȡ�ص����ݣ�ͨ����ȫ�ġ������������ʽ�����ͻ���
    *out_value = value;

	// void * 
    // memcpy(out_value, &value, sizeof(int));

    // ���� 4: ��������ɹ���
    return true;
}

size_t sarray_get_capacity(const SafeArray* sarray) {
    if (!sarray) return 0;
    return sarray->capacity;
}

void sarray_print(const SafeArray* sarray) {
    if (!sarray) {
        printf("SafeArray is NULL.\n");
        return;
    }
    printf("SafeArray (capacity: %zu) [ ", sarray->capacity);
    for (size_t i = 0; i < sarray->capacity; i++) {
        // Ϊ�˴�ӡ��������������ס�ֱ�ӷ������ڲ����ݡ�
        // ��ʵ��Ӧ���У������������Բ��ṩ��ӡ�������Ա�֤���Եķ�װ��
        printf("%d ", sarray->data[i]);
    }
    printf("]\n");
}