/**
 * CSS 430
 * Project 3 - First Come First Serve Scheduling Algorithm
 * Created by: Helina Azer
 */

#include "task.h"
#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node *list = NULL;

//function that adds the tasks to the list
void add(char *name, int priority, int burst) {
  Task *task = malloc(sizeof(Task));
  task->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(task->name, name);
  task->priority = priority;
  task->burst = burst;
  insert(&list, task);
}

//function to compare the tasks so they can be sorted in lexicographical order
bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

//function that chooses which task to go next based on priority. Sorts based on Lexicographical order
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!list)
    return NULL;
  struct node *temp = list;
  Task *best_sofar = temp->task;
  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&list, best_sofar);
  return best_sofar;
}

//function that schedules the task to be printed.
void schedule() {
  int totalTime = 0;
  while(list) {
    Task *currentTask = pickNextTask();
    run(currentTask, currentTask->burst);
    totalTime += currentTask->burst;
    printf("        Time is now: %d \n", totalTime);
  }
}