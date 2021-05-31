/*
 ============================================================================
 Name        : CWK2Q4.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement your own XOR Linked List (https://en.wikipedia.org/wiki/XOR_linked_list)
 in C capable of storing names. Your implementation should have the following
 functions:
    void insert_string(const char* newObj)
	int insert_before(const char* before, const char* newObj)
	int insert_after(const char* after, const char* newObj)
	void remove_string(char* result)
	int remove_after(const char *after, char *result)
	int remove_before(const char *before, char *result)
    void print_list()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct Node;
typedef struct Node *Node;
struct Node
{
	Node xor ;	// XOR or previous and next nodes.
	char *data; // Contents of the node.
};
Node head = NULL;

/* FUNCTIONS */
void insert_string(const char *newObj);
int insert_before(const char *before, const char *newObj);
int insert_after(const char *after, const char *newObj);
int remove_string(char *result);
int remove_after(const char *after, char *result);
int remove_before(const char *before, char *result);
Node XOR(Node previous, Node next);
int is_too_long(const char *newObj);
void print_list();

int main(int argc, char *argv[])
{
	insert_string("Alpha");
	insert_string("Bravo");
	insert_string("Charlie");
	insert_after("Bravo", "Delta");
	insert_before("Alpha", "Echo");
	print_list(); // Charlie -> Bravo -> Delta -> Echo -> Alpha

	char result[64];
	int ret;

	ret = remove_after("Delta", result);
	if (ret)
		printf("Removed: %s\n", result);
	print_list();

	ret = remove_before("Bravo", result);
	if (ret)
		printf("Removed: %s\n", result);
	print_list();

	ret = remove_string(result);
	if (ret)
		printf("Removed: %s\n", result);
	print_list();
}

/*
* Inserts a string into the linked list at the beginning.
*
* @param newObj The string to be inserted into the linked list.
*/
void insert_string(const char *newObj)
{
	if (is_too_long(newObj))
		return;
	Node new_node = malloc(sizeof(struct Node));
	new_node->data = (char *)newObj;
	new_node->xor = head;

	if (head != NULL)
	{
		head->xor = XOR(new_node, head->xor);
	}
	head = new_node;
}

/*
* Inserts a string into the linked list before another string, it it is in the list.
*
* @param newObj The string to be inserted into the linked list.
* @param before The the string that newObj is to be inserted before.
* @return 1 if the insertion was successful, 0 otherwise.
*/
int insert_before(const char *before, const char *newObj)
{
	if (is_too_long(newObj))
		return 0;
	Node previous = NULL, current = head, next = XOR(previous, current->xor);

	while (current)
	{
		if (strcmp(before, current->data) == 0)
		{
			if (previous == NULL)
			{
				insert_string(newObj);
				return 1;
			}
			Node new_node = malloc(sizeof(struct Node));
			new_node->data = (char *)newObj;

			current->xor = XOR(new_node, next);
			previous->xor = XOR(XOR(previous->xor, current), new_node);
			new_node->xor = XOR(previous, current);
			return 1;
		}
		previous = current;
		current = next;
		next = XOR(previous, current->xor);
	}
	return 0;
}

/*
* Inserts a string into the linked list after another string, it it is in the list.
*
* @param newObj The string to be inserted into the linked list.
* @param before The the string that newObj is to be inserted after.
* @return 1 if the insertion was successful, 0 otherwise.
*/
int insert_after(const char *after, const char *newObj)
{
	if (is_too_long(newObj))
		return 0;
	Node previous = NULL, current = head, next = XOR(previous, current->xor);

	while (current)
	{
		if (strcmp(after, current->data) == 0)
		{
			Node new_node = malloc(sizeof(struct Node));
			new_node->data = (char *)newObj;

			current->xor = XOR(previous, new_node);
			if (next != NULL)
			{
				next->xor = XOR(new_node, XOR(current, next->xor));
			}
			new_node->xor = XOR(current, next);

			return 1;
		}
		previous = current;
		current = next;
		next = XOR(previous, current->xor);
	}
	return 0;
}

/*
* Removes the string from the head of the list.
*
* @param result The string removed from the list.
* @return 1 if the removal was successful, 0 otherwise.
*/
int remove_string(char *result)
{
	if (head == NULL)
		return 0;
	strcpy(result, head->data);

	Node old_head = head;
	head = XOR(NULL, head->xor);
	head->xor = XOR(NULL, XOR(old_head, head->xor));
	free(old_head);
	return 1;
}

/*
* Removes a node from the list after a specified string.
*
* @param after The string of the node after the one to be removed.
* @param result The string removed from the list.
* @return 1 if the removal was successful, 0 otherwise.
*/
int remove_after(const char *after, char *result)
{
	Node previous = NULL, current = head, next = XOR(previous, current->xor);

	while (current)
	{
		if (strcmp(after, current->data) == 0)
		{
			if (next == NULL)
				return 0;
			strcpy(result, next->data);
			Node after_next = XOR(current, next->xor);
			//TODO error checking
			current->xor = XOR(previous, after_next);
			after_next->xor = XOR(current, XOR(next, after_next->xor));
			free(next);
			return 1;
		}
		previous = current;
		current = next;
		next = XOR(previous, current->xor);
	}
	return 0;
}

/*
* Removes a node from the list before a specified string.
*
* @param before The string of the node before the one to be removed.
* @param result The string removed from the list.
* @return 1 if the removal was successful, 0 otherwise.
*/
int remove_before(const char *before, char *result)
{
	Node previous = NULL, current = head, next = XOR(previous, current->xor);

	while (current)
	{
		if (strcmp(before, current->data) == 0)
		{
			if (previous == NULL)
				return 0;
			strcpy(result, previous->data);
			Node before_previous = XOR(current, previous->xor);
			if (before_previous == NULL)
			{
				remove_string(result);
				return 1;
			}
			//TODO error checking
			current->xor = XOR(before_previous, next);
			before_previous->xor = XOR(XOR(before_previous->xor, previous), current);
			free(previous);
			return 1;
		}
		previous = current;
		current = next;
		next = XOR(previous, current->xor);
	}
	return 0;
}

/*
* Computes the XOR of the pointers to two nodes.
*
* @param a,b A pointer to a node to be xored
* @return The XOR of the pointers to each node.
*/
Node XOR(Node a, Node b)
{
	return (Node)((uintptr_t)a ^ (uintptr_t)b);
}

/*
* Determines whether a string is too long to be inserted into the linked list.
*
* @param newObj The string to be inserted into the linked list.
* @return 1 if the string is too long to be inserted into the linked list, 0 otherwise.
*/
int is_too_long(const char *newObj)
{
	return strlen(newObj) + 1 > 64;
}

/*
* Prints the linked list.
*/
void print_list()
{
	Node previous = NULL;
	Node current = head;
	Node next = XOR(previous, current->xor);
	while (next)
	{
		printf("%s -> ", current->data);
		previous = current;
		current = next;
		next = XOR(previous, current->xor);
	}
	printf("%s", current->data);
	printf("\n");
}
