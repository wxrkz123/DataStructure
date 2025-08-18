#include <stdio.h>
#include "linked_stack.h"

// ����һ���Զ���ṹ�����ڲ���
typedef struct {
	int id;
	char name[20];
} Record;

void print_record(const Record* r) {
	if (r) {
		printf("Record(id: %d, name: \"%s\")", r->id, r->name);
	}
}

void test_record_stack() {
	printf("--- Testing Struct (Record) Linked Stack ---\n");
	// ����ʱ������Ҫ����
	Stack* record_stack = stack_create(sizeof(Record));

	if (!record_stack) {
		printf("Failed to create record stack.\n");
		return;
	}

	printf("Stack created. Is empty? %s\n", stack_is_empty(record_stack) ? "Yes" : "No");

	// ����һЩ���ݲ�ѹջ
	Record r1 = { 1, "Alice" };
	Record r2 = { 2, "Bob" };
	Record r3 = { 3, "Charlie" };

	printf("Pushing "); print_record(&r1); printf("...\n");
	stack_push(record_stack, &r1);

	printf("Pushing "); print_record(&r2); printf("...\n");
	stack_push(record_stack, &r2);

	printf("Pushing "); print_record(&r3); printf("...\n");
	stack_push(record_stack, &r3);

	printf("Current stack size: %zu\n", stack_get_size(record_stack));

	Record peeked_record;
	if (stack_peek(record_stack, &peeked_record)) {
		printf("Peek at top: ");
		print_record(&peeked_record);
		printf("\n");
	}

	printf("\nPopping all elements:\n");
	while (!stack_is_empty(record_stack)) {
		Record popped_record;
		if (stack_pop(record_stack, &popped_record)) {
			printf("Popped: ");
			print_record(&popped_record);
			printf(" | New size: %zu\n", stack_get_size(record_stack));
		}
	}

	printf("\nIs stack empty now? %s\n", stack_is_empty(record_stack) ? "Yes" : "No");

	// ����ջ
	stack_destroy(&record_stack);
	printf("Stack destroyed. Pointer is now %s\n", record_stack == NULL ? "NULL" : "Not NULL");
}


// ------------------------�������ݵĺ����Ǻ���Ŀγ�--------------------------------------
// ��������Ƿ�ƥ��ĸ�������
bool is_opener(char c) {
	return c == '(' || c == '{' || c == '[';
}

bool matches(char opener, char closer) {
	return (opener == '(' && closer == ')') ||
		(opener == '{' && closer == '}') ||
		(opener == '[' && closer == ']');
}

bool check_brackets(const char* code) {

	Stack* stack = stack_create(sizeof(char));

	if (!stack) {

		fprintf(stderr, "Failed to create stack for bracket checking.\n");
		return false;
	}

	size_t len = strlen(code);
	for (size_t i = 0; i < len; i++) {
		char current_char = code[i];

		//  ����ǿ����ţ���ѹ��ջ
		if (is_opener(current_char)) {
			stack_push(stack, &current_char);

			// ����Ǳ����ţ�����ջ��Ԫ��
		}
		else if (current_char == ')' || current_char == '}' || current_char == ']') {

			// �������1��ջΪ�գ������Ŷ���
			if (stack_is_empty(stack)) {
				fprintf(stderr, "Unmatched closing bracket '%c' found.\n", current_char);
				stack_destroy(&stack);
				return false; // �����Ŷ���
			}

			char popped_opener;
			// ��ջ������һ�������Ž��бȽ�
			stack_pop(stack, &popped_opener);// ��������char����popped_opener

			// �������2�������źͱ����Ų�ƥ�� �����Ͳ�ƥ�䣩
			if (!matches(popped_opener, current_char)) {

				fprintf(stderr, "Mismatched brackets: '%c' does not match '%c'.\n", popped_opener, current_char);
				stack_destroy(&stack);
				return false; // �������Ų�ƥ��
			}
		}
	}

	// �������3����������������ջ����Ȼ��δ�رյĿ�����
	if (!stack_is_empty(stack)) {

		char unclosed_opener;

		// �鿴һ������һ��������û�йر�
		stack_peek(stack, &unclosed_opener);
		fprintf(stderr, "Unmatched opening brackets remain in stack.\n");
		stack_destroy(&stack);
		return false; // ��δ�رյĿ�����
	}


	// ����ջ
	stack_destroy(&stack);
	return true;
}

void check() {
	const char* test_cases[] = {
	   "int main() { int x = (1 + 2); return 0; }", // ��Ч
	   "void func(int a[]);",                        // ��Ч
	   "([{}])",                                     // ��Ч
	   "int arr[5] = {1, 2, 3};",                     // ��Ч
	   "",                                           // ��Ч (���ַ���)
	   "abc",                                        // ��Ч (������)
	   "([)]",                                       // ��Ч: ���治ƥ��
	   "((()",                                       // ��Ч: ������δ�ر�
	   "())",                                        // ��Ч: �������޶�Ӧ
	   "if (x > 0) { printf(\"hello\");",             // ��Ч: } δ�ر�
	   "int y = { ( [ ] ) };"                         // ��Ч: ����Ƕ��
	};

	int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

	for (int i = 0; i < num_cases; i++) {
		printf("���ڼ��: \"%s\"\n", test_cases[i]);
		if (check_brackets(test_cases[i])) {
			printf("���: -> ��Ч\n\n");
		}
		else {
			printf("���: -> ��Ч\n\n");
		}
	}
}

int main() {
	// ---------------������ʽջ--------------------�����ע��ȡ������--------------------
	// test_record_stack();


	// ------------------------�������ݵĺ����Ǻ���Ŀγ�--------------------------------------

	// ��������ƥ����������Ŀγ̣�
	check();

	return 0;
}