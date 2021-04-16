#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <string.h>

/*---------------------------Defines---------------------------------*/

#define DESCMAX 50
#define USERMAX 20 + 1
#define CARMAX 20 + 1
#define ACTMAX 10
#define MAXINPUT 419710
#define TASKMAX 10000
#define DIFFUSERMAX 50

typedef struct{
  int started;
  int id; 
  char desc[DESCMAX];
  char user[USERMAX];
  char act[CARMAX];
  int timeRequested;
  int timePostStart;
}task;

typedef struct {
  char username[USERMAX];
}user;

typedef struct{
  char activity[CARMAX];
}act;

/*---------------------------------Global-Vars--------------------------------*/

task taskBank[TASKMAX];
int tasksUsed = 1, time = 0, userUsed = 0, actUsed = 3;

/*--------------------------------Functions-----------------------------------*/

int addTask(int id, char arguments[]);
void taskList(char arguments[], int tasksUsed);
int timeForward(char arguments[]);
int addUser(char arguments[], user userBank[]);
void moveTask(user userBank[], char arguments[], act actBank[]);
void actList(char arguments[], act actBank[ACTMAX]);
int addAct(char arguments[], act actBank[]);
int * sortElements(int tasksUsed, int * IDs);
void changingElems(int a[] , int i, int h);
int partition(int IDs[], int leftLim, int rightLim, int isDesc);
int less(int arg1, int arg2, int isDesc);
void quicksort(int IDs[], int leftLim, int rightLim, int isDesc);

/*----------------------------Main-----------------------------------*/

int main() {
  char command, arguments[MAXINPUT];
  user userBank[DIFFUSERMAX];
  act actBank[ACTMAX];

  strcpy(actBank[0].activity, "TO DO");
  strcpy(actBank[1].activity, "IN PROGRESS");
  strcpy(actBank[2].activity, "DONE");

  while ((command = getchar()) != 'q') {
    fgets(arguments, MAXINPUT, stdin);
    switch (command) {
    case 't':
      tasksUsed = addTask(tasksUsed, arguments);
      break;
    case 'l':
      taskList(arguments, tasksUsed);
      break;
    case 'n':
      time = timeForward(arguments);
      break;
    case 'u':
      userUsed = addUser(arguments, userBank);
      break;
    case 'm':
      moveTask(userBank, arguments, actBank);
      break;
    case 'd':
      actList(arguments, actBank);
      break;
    case 'a':
      actUsed = addAct(arguments, actBank);
      break;
    default:
      printf("Error: No such command\n");
    }
  }
  return 0;
}

int addTask(int id, char arguments[]) {
  char descript[DESCMAX];
  int time, i, val = 0;

  sscanf(arguments, "%d %[^\n]", & time, descript);
  for (i = 0; i < id; ++i)
    if (!strcmp(taskBank[i].desc, descript))
      val = 1;
  if (id > TASKMAX) {
    printf("too many tasks\n");
    return id;
  } else if (val) {
    printf("duplicate description\n");
    return id;
  } else if (time <= 0) {
    printf("invalid duration\n");
    return id;
  } else {
    taskBank[id - 1].id = id;
    taskBank[id - 1].timeRequested = time;
    strcpy(taskBank[id - 1].act, "TO DO");
    strcpy(taskBank[id - 1].desc, descript);
    printf("task %d\n", id);
  }
  return ++id;
}

void taskList(char arguments[], int tasksUsed) {
  int i = 0, inputid, j, reset, toSort[TASKMAX];
  char jar[MAXINPUT], numstr[5], pote[5];

  strcpy(jar, arguments);
  if (jar[0] == '\n') {
    for (i = 0; i < tasksUsed - 1; ++i)
      toSort[i] = i;
    quicksort(toSort, 0, tasksUsed-2, 1);
    for (i = 0; i < tasksUsed - 1; ++i)
      printf("%d %s #%d %s\n", taskBank[toSort[i]].id, taskBank[toSort[i]].act,
        taskBank[toSort[i]].timeRequested, taskBank[toSort[i]].desc);
  } else {
    while (jar[i] != '\n') {
      if ((jar[i] != ' ') && (jar[i] != '\t')) {
        pote[0] = jar[i];
        j = 0;
        while ((jar[i + j] != ' ') && (jar[i + j] != '\n') && (jar[i + j] != '\t')) {
          pote[j] = jar[i + j];
          ++j;
        }
        if (j != 0)
          i += j - 1;
        strcpy(numstr, pote);
        inputid = atoi(numstr);
        --inputid;
        if (!taskBank[inputid].id || inputid < 0)
          printf("%d: no such task\n", inputid + 1);
        else
          printf("%d %s #%d %s\n", taskBank[inputid].id, taskBank[inputid].act,
            taskBank[inputid].timeRequested, taskBank[inputid].desc);
        for (reset = 0; reset < 6; ++reset)
          pote[reset] = '\0';
      }
      ++i;
    }
  }
}

int timeForward(char arguments[]) {
  int tint;

  sscanf(arguments, "%d", & tint);
  if (tint >= 0) {
    printf("%d\n", time += tint);
  } else
    printf("invalid time\n");
  return time;
}

int addUser(char username[], user userBank[]) {
  int i = 0, count = 0, j = 0;
  char jar[USERMAX];

  if (username[0] != '\n') {
    while (username[count] == ' ' || username[count] == '\t') {
      ++count;
    }
    while (username[i] != '\n') {
      if (i >= count) {
        jar[j] = username[i];
        j += 1;
      }
      ++i;
    }
    jar[j] = '\0';

    for (i = 0; i < userUsed; ++i)
      if (!strcmp(userBank[i].username, jar)) {
        printf("user already exists\n");
        return userUsed;
      }
    if (userUsed == DIFFUSERMAX) {
      printf("too many users\n");
      return userUsed;
    }
    strcpy(userBank[userUsed].username, jar);
    return ++userUsed;
  } else {
    if (userUsed != 0)
      for (i = 0; i < userUsed; ++i)
        printf("%s\n", userBank[i].username);
    return userUsed;
  }
}

void moveTask(user userBank[], char arguments[], act actBank[]) {

  int idRequested, val = 1, i, j = 0, h = 0;
  char userRequested[USERMAX], actRequested[CARMAX];

  sscanf(arguments, "%d %s %[^\n]", & idRequested, userRequested, actRequested);

  if (!strcmp(actRequested, taskBank[idRequested-1].act)) return;


  for (i = 0; i < userUsed; ++i)
    if (!strcmp(userBank[i].username, userRequested))
      ++j;
  for (i = 0; i < actUsed; ++i)
    if (!strcmp(actBank[i].activity, actRequested))
      ++h;

  if (idRequested >= tasksUsed || idRequested <= 0) {
    printf("no such task\n");
    val = 0;
  } else if (!strcmp(actRequested, "TO DO") && strcmp(taskBank[idRequested-1].act, "TO DO")) {
    printf("task already started\n");
    val = 0;
  } else if (!j && val)
    printf("no such user\n");
  else if (!h && val)
    printf("no such activity\n");

  if (j && h && val) {
    if (!strcmp(actRequested, "DONE")) {
      taskBank[idRequested - 1].started = 0;
      if (!strcmp(taskBank[idRequested-1].act, "TO DO")) taskBank[idRequested - 1].timePostStart = time;
      printf("duration=%d slack=%d\n", time - taskBank[idRequested - 1].timePostStart,
        time - taskBank[idRequested - 1].timePostStart - taskBank[idRequested - 1].timeRequested);
      strcpy(taskBank[idRequested - 1].act, "DONE");
    } else {
      if (!strcmp(taskBank[idRequested-1].act, "TO DO")){
        taskBank[idRequested-1].timePostStart = time;
      }
      strcpy(taskBank[idRequested - 1].act, actRequested);
      taskBank[idRequested - 1].started = 1;
    }
  }
}

void actList(char arguments[], act actBank[ACTMAX]) {
  int i = 0, result = 1, j = 0, h[TASKMAX], count = 0, pote[TASKMAX], printer, * sortedDesc, z;
  char jar[CARMAX];

  while (arguments[i] != '\n') {
    if (i != 0)
      jar[i - 1] = arguments[i];
    ++i;
  }
  jar[i - 1] = '\0';

  for (i=0; i <= ACTMAX; ++i)
    if (!(strcmp(actBank[i].activity, jar)))
      result = 0;

  if (result)
    printf("no such activity\n");
  else {
    for (i = 0; i < TASKMAX; ++i)
      if (!strcmp(jar, taskBank[i].act)) {
        h[j] = i;
        ++j;
      }
    if (j != 0) {
      quicksort(h, 0, j-1, 0);
      for (i = 0; i < j; ++i) { /* dentro do sortedint vamos percorrer cada elemento*/
        pote[0] = h[i];
        z = 1;
        for (count = i + 1; count < j && taskBank[pote[0]].timePostStart == taskBank[h[count]].timePostStart; ++count) {
            pote[z] = h[count];
            ++z;
        }
        i += z-1;
        sortedDesc = sortElements(z+1, pote);
        for (printer = 0; printer < z; ++printer)
            printf("%d %d %s\n", taskBank[sortedDesc[printer]].id,
              taskBank[sortedDesc[printer]].timePostStart, taskBank[sortedDesc[printer]].desc);
      }
    }
  }
}

int addAct(char arguments[], act actBank[]) {
  int i = 0;
  char jar[CARMAX];

  if (arguments[0] != '\n') {
    if (actUsed == ACTMAX) {
      printf("too many activities\n");
      return actUsed;
    }
    while (arguments[i] != '\n') {
      if (i != 0)
        jar[i - 1] = arguments[i];
      ++i;
    }
    jar[i - 1] = '\0';
    i = 0;
    while (jar[i] != '\0') {
      if (jar[i] >= 'a' && jar[i] <= 'z') {
        printf("invalid description\n");
        return actUsed;
      }
      ++i;
    }
    for (i = 0; i < ACTMAX; ++i)
      if (!strcmp(jar, actBank[i].activity)) {
        printf("duplicate activity\n");
        return actUsed;
      }
    strcpy(actBank[actUsed].activity, jar);
    return ++actUsed;
  } else
    for (i = 0; i < actUsed; ++i)
      printf("%s\n", actBank[i].activity);
  return actUsed;
}

void changingElems(int a[] , int i, int h){
  int holder;
  
  holder = a[i];
  a[i] = a[h];
  a[h] = holder;
}

int less(int arg1, int arg2, int isDesc){
  if (isDesc) return (strcmp(taskBank[arg1].desc, taskBank[arg2].desc) < 0);
  else return (taskBank[arg1].timePostStart < taskBank[arg2].timePostStart);
}

int * sortElements(int tasksUsed, int * IDs) {
  int jar, i, h, holder;

  for (i = 1; i < tasksUsed-1; ++i) {
    h = i - 1;
    jar = i;
    while (h >= 0 
    && (strcmp(taskBank[IDs[jar]].desc, taskBank[IDs[h]].desc) < 0)) {
      holder = IDs[jar];
      IDs[jar] = IDs[h];
      IDs[h] = holder;
      --jar;
      --h;
    }
  }
  return IDs;
}

int partition(int IDs[], int leftLim, int rightLim, int isDesc) {
  int i = leftLim-1;
  int j = rightLim;
  int v = IDs[rightLim];
  while (i < j) {
    while (less(IDs[++i], v, isDesc));
    while (less(v, IDs[--j], isDesc))
      if (j == leftLim)
        break;
    if (i < j)
      changingElems(IDs, i, j);
  }
  changingElems(IDs, rightLim, i);
  return i;
}

void quicksort(int IDs[], int leftLim, int rightLim, int isDesc) {
  int i;
  if (rightLim <= leftLim) return;
  i = partition(IDs, leftLim, rightLim, isDesc);
  quicksort(IDs, leftLim, i-1, isDesc);
  quicksort(IDs, i+1, rightLim, isDesc);
}