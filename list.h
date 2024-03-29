/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
int size(struct node *list);
void delete(struct node **head, Task *task);
void traverse(struct node *head);