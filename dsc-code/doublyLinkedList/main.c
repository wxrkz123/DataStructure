#define _CRT_SECURE_NO_WARNINGS
#include "DoublyLinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	int id;
	char* name;

} Employee;

// Helper function to duplicate a string
char* protable_strdup(const char* str) {
	if (!str) return NULL;
	size_t len = strlen(str);
	char* new_str = (char*)malloc(len + 1);
	if (new_str) {
		strcpy(new_str, str);
	}
	return new_str;
}

void free_employee(void* data) {

	Employee* emp = (Employee*)data;

	if (emp) {
		printf("Freeing employee ID: %d, Name: %s\n", emp->id, emp->name);
		free(emp->name); // Free the dynamically allocated name
		free(emp);       // Free the employee structure itself
	}
}

int compare_employee_by_id(const void* a, const void* b) {
	const Employee* emp_a = (const Employee*)a;
	const Employee* emp_b = (const Employee*)b;
	return emp_a->id - emp_b->id; // Compare by ID
}

void print_employee_action(void* data, void* context) {
	const Employee* emp = (const Employee*)data;

	// context is not used here, but you can use it if needed
	// TODO 

	if (!emp) {
		printf("Employee data is NULL.\n");
		return; // Check if emp is NULL
	}
	printf("Employee ID: %d, Name: %s\n", emp->id, emp->name);
}

int main(void) {

	printf("--- Architecture of Doubly Linked List ---\n");

	// 1. Create a new doubly linked list with a memory pool
	printf("Setup 1: Creating a list with a memory pool of 10 nodes...\n");

	DoublyLinkedList* emp_list = List_Create(10, free_employee);

	if (!emp_list) {
		fprintf(stderr, "Failed to create the doubly linked list.\n");
		return 1; // Exit if list creation failed
	}

	printf("List created successfully. Initial size: %zu\n", List_GetSize(emp_list));

	// 2. Append some employees to the list

	printf("Setup 2: Appending employees to the list...\n");

	for (int i = 0; i < 5; i++) {
		Employee* emp = (Employee*)malloc(sizeof(Employee));
		if (emp == NULL) {
			fprintf(stderr, "Memory allocation failed for employee.\n");
			continue; // Skip this iteration if memory allocation fails
		}
		emp->id = 101 + i;
		char buffer[50];
		sprintf(buffer, "Employee #%d", emp->id);
		emp->name = protable_strdup(buffer);
		if (emp->name == NULL) {
			fprintf(stderr, "Memory allocation failed for employee name.\n");
			free(emp); // Free the employee structure if name allocation fails
			continue;
		}
		List_Append(emp_list, emp);
	}

	// 3. Print the list of employees
	printf("Setup 3: Printing the list of employees...\n");
	List_ForEach(emp_list, print_employee_action, NULL);
	printf("Current list size: %zu\n", List_GetSize(emp_list));

	// 4. Find an employee by ID

	printf("Setup 4: Finding an employee with ID 103...\n");

	Employee search_key = { 103, NULL }; // Create a search key with ID 103

	DListNode* found_node = List_Find(emp_list, &search_key, compare_employee_by_id);

	if (found_node) {
		Employee* found_emp = (Employee*)List_GetData(found_node);
		printf(": Found employee with ID %d, Name: %s\n", found_emp->id, found_emp->name);
	} else {
		printf("Employee with ID 103 not found.\n");
	}

	// 5. Delete an employee from the list
	if (found_node) {
		printf("Setup 5: Deleting employee with ID 103...\n");
		List_DeleteNode(emp_list, found_node);
		printf("Employee with ID 103 deleted. Current list size: %zu\n", List_GetSize(emp_list));
	}

	printf("\n --- List fater deletion---\n");

	List_ForEach(emp_list, print_employee_action, NULL);

	// 6. insert value -> head node

	printf("Setup 6: Prepending an employee to the list...\n");
	Employee* ceo = (Employee*)malloc(sizeof(Employee));

    // Before using the 'ceo' pointer, ensure it is not NULL
    if (ceo != NULL) {
        ceo->id = 99;
        ceo->name = protable_strdup("CEO");
        if (ceo->name == NULL) {
            fprintf(stderr, "Memory allocation failed for CEO name.\n");
            free(ceo); // Free the employee structure if name allocation fails
        } else {
            List_Prepend(emp_list, ceo);
            printf("Prepended employee with ID %d, Name: %s\n", ceo->id, ceo->name);
			printf("\n--- Final list contents ---\n");
			List_ForEach(emp_list, print_employee_action, NULL);
			printf("Final list size: %zu\n", List_GetSize(emp_list));
        }
    } else {
        fprintf(stderr, "Memory allocation failed for CEO.\n");
    }



	// 7. Clean up the list
	printf("Cleanup: Destroying the doubly linked list...\n");

	List_Destroy(&emp_list);
	printf("List destroyed. The pointer emp_list is now NULL: %s\n", 
		emp_list == NULL ? "true" : "false");

	printf("All resources freed successfully.\n");

	return 0;
}