/**
 * CSS 430
 * Project 3 - Shortest Job First Scheduling Algorithm
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
  if (!list) {
      return NULL;
  }
  struct node *temp = list;
  Task *shortest = temp->task;
  while (temp != NULL) {
    if (temp->task->burst == shortest->burst) {
      if (comesBefore(temp->task->name, shortest->name)) {
        shortest = temp->task;
      }
    } else if (temp->task->burst < shortest->burst) {
      shortest = temp->task;
    }
    temp = temp->next;
  }
  delete(&list, shortest);
  return shortest;
}

//function that schedules the task to be printed.
void schedule() {
  int totalTime = 0;
  while(list) {
    Task *currTask = pickNextTask();
    run(currTask, currTask->burst);
    totalTime += currTask->burst;
    printf("        Time is now: %d \n", totalTime);
  }
}