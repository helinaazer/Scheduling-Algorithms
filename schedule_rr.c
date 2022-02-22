/**
 * CSS 430
 * Project 3 - Round Robin Scheduling Algorithm
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
Task *pickNextTask () {
  if (!list) {
    return NULL;
  }
  struct node *temp = list;
  Task *best_sofar = temp->task;
  while (temp != NULL) {
    if (temp->task->t == best_sofar->t) {
      if (comesBefore(temp->task->name, best_sofar->name)) {
        best_sofar = temp->task;
        temp = temp->next;
        continue;
      }
    } else if (temp->task->t < best_sofar->t) {
      best_sofar = temp->task;
      temp = temp->next;
      continue;
    }
    temp = temp->next;
  }
  return best_sofar;
}

//function that schedules the task to be printed.
void schedule () {
  int totalTime = 0;
  Task *currentTask = pickNextTask();
  while (currentTask) {
    currentTask->t += 1;
    //if the burst is more than the quantum then do in increments of time quantum
    if (currentTask->burst > QUANTUM) {
      totalTime += QUANTUM;
      run(currentTask, QUANTUM);
      currentTask->burst -= QUANTUM;
    //if the burst is less than or equal to quantum then do the remaining burst
    } else  {
      totalTime += currentTask->burst;
      run(currentTask, currentTask->burst);
      delete(&list, currentTask);
    }
    printf("        Time is now: %d \n", totalTime);
    currentTask = pickNextTask();
  }
}
