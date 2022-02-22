/**
 * CSS 430
 * Project 3 - Priority Round Robin Scheduling Algorithm
 * Created by: Helina Azer
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *list[MAX_PRIORITY+1];

//function that adds the tasks to the list
void add(char *name, int priority, int burst) {
  Task *task = malloc(sizeof(Task));
  task->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(task->name, name);
  task->priority = priority;
  task->burst = burst;
  insert(&list[priority], task);
}

//function to compare the tasks so they can be sorted in lexicographical order
bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

//function that chooses which task to go next based on priority. Sorts based on Lexicographical order
Task *pickNextTask(struct node *head) {
    if (!head) {
        return NULL;
    }
    struct node *temp = head;
    Task *high = temp->task;
    while (temp != NULL) {
        //if the priorities are the same, then go with lexicographical order
        if (temp->task->priority == high->priority) {
            if (comesBefore(temp->task->name, high->name)) {
                high = temp->task;
            }
        //if the priority of one task is greater, then do that task first
        } else if (temp->task->priority > high->priority) {
            high = temp->task;
        }
        temp = temp->next;
    }
    return high;
}

//function that chooses which tasks to go next based on round robin scheduling. Sorts based on lexicographical order
Task *pickNextTask_rr(struct node *head) {
  if (!head) {
    return NULL;
  }
  struct node *temp = head;
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
void schedule() {
    int totalTime = 0;
    for (int priority = MAX_PRIORITY; priority >= MIN_PRIORITY; priority--) {
        while (list[priority]) {
            //if more that one burst has the same priority, do round robin
            if (size(list[priority]) > 1) {
                Task *currentTask = pickNextTask_rr(list[priority]);
                while(currentTask) {
                    currentTask->t += 1;
                    if(currentTask->burst <= QUANTUM) {
                        totalTime += currentTask->burst;
                        run(currentTask, currentTask->burst);
                        delete(&list[priority], currentTask);
                    } else {
                        totalTime += QUANTUM;
                        run(currentTask, QUANTUM);
                        currentTask->burst -= QUANTUM;
                    }
                    printf ("        Time is now: %d \n", totalTime);
                    currentTask = pickNextTask_rr(list[priority]);
                }
            //if there is only one burst for that priority, pick the priority
            } else {
                Task *currentTask = pickNextTask(list[priority]);
                while(currentTask) {
                    run(currentTask, currentTask->burst);
                    totalTime += currentTask->burst;
                    delete(&list[priority], currentTask);
                    currentTask = pickNextTask(list[priority]);
                    printf("        Time is now: %d \n", totalTime);
                }
            }
        }
    }
}