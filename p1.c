#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <string.h>

/*---------------------------Defines---------------------------------*/

#define DESCMAX 50
#define USERMAX 20 + 1
#define CARMAX 20 + 1
#define ACTMAX 10
#define MAXINPUT 388850 /*ver como e que eu posso mudar isto*/
#define TASKMAX 10000
#define DIFFUSERMAX 50

#define less(A, B) strcmp(A, B) < 0
#define iqual(A, B) strcmp(A, B) == 0


struct task {
  int started; /*If the task has left the TO DO activity */
  int id; /*The ID of the task */
  char desc[DESCMAX]; /*The unique description of the task*/  
  char act[CARMAX];  /*The activity that the task belongs (starts at "TO DO")*/
  int timeRequested; /*The time that the user requested to finnish the task*/
  int timePostStart; 
  /*The time that the task is taking to be done after leaving "TO DO" activity*/ 
};

struct user {
  char username[USERMAX]; /*username given by the user*/
};

struct act {
  char activity[CARMAX]; /*activity name given by the user*/
};

/*----------------------------Global-Vars----------------------------*/

struct task taskBank[TASKMAX];
int time=0; /*global system time */

/*---------------------------Functions-------------------------------*/

int eval(char c);
int addTask(int id, char arguments[]);
void taskList(int id, char arguments[]);
int timeForward(char arguments[]);
int addUser(char arguments[], struct user userBank[], int userUsed);
void moveTask(int tasksUsed, struct user userBank[],
  char arguments[], struct act actBank[]);
void actList(char arguments[], struct act actBank[ACTMAX]);
int addAct(int actUsed, char arguments[], struct act actBank[]);
int * sortElements(int tasksUsed, int * IDs, int isDesc);

/*----------------------------Main-----------------------------------*/

int main() {
  char command, arguments[MAXINPUT];
  int tasksUsed = 1, userUsed = 0, actUsed = 3;
  struct user userBank[DIFFUSERMAX];
  struct act actBank[ACTMAX];

  /*Creation of the default activities */ 
  strcpy(actBank[0].activity, "TO DO");
  strcpy(actBank[1].activity, "IN PROGRESS"); 
  strcpy(actBank[2].activity, "DONE");

  while ((command = getchar()) != 'q') {
    fgets(arguments, MAXINPUT, stdin);
    if (eval(command))
      switch (command) {
      case 't':
        tasksUsed = addTask(tasksUsed, arguments);
        break;
      case 'l':
        taskList(tasksUsed, arguments);
        break;
      case 'n':
        time = timeForward(arguments);
        break;
      case 'u':
        userUsed = addUser(arguments, userBank, userUsed);
        break;
      case 'm':
        moveTask(tasksUsed, userBank, arguments, actBank);
        break;
      case 'd':
        actList(arguments, actBank);
        break;
      case 'a':
        actUsed = addAct(actUsed, arguments, actBank);
        break;
      }
    else
      printf("ups!\n");
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

void taskList(int id, char arguments[]) {
  int i = 0, inputid, j, reset, toSort[TASKMAX], * sorted;
  char jar[MAXINPUT], numstr[5], pote[5];

  strcpy(jar, arguments);
  if (jar[0] == '\n') {
    for (i = 0; i < id - 1; ++i)
      toSort[i] = i;
    sorted = sortElements(id, toSort, 1);
    for (i = 0; i < id - 1; ++i)
      printf("%d %s #%d %s\n", taskBank[sorted[i]].id, taskBank[sorted[i]].act,
        taskBank[sorted[i]].timeRequested, taskBank[sorted[i]].desc);
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

int addUser(char username[], struct user userBank[], int userUsed) {
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

    for (i = 0; i < DIFFUSERMAX; ++i)
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

void moveTask(int tasksUsed, struct user userBank[],
  char arguments[], struct act actBank[]) {

  int idRequested, val = 1, i, j = 0, h = 0;
  char userRequested[USERMAX], actRequested[CARMAX];

  sscanf(arguments, "%d %s %[^\n]", & idRequested, userRequested, actRequested);

  if (iqual(actRequested, taskBank[idRequested-1].act))
    return;


  for (i = 0; i < DIFFUSERMAX; ++i)
    if (!iqual(userBank[i].username, userRequested))
      j = 1;
  for (i = 0; i < ACTMAX; ++i)
    if (iqual(actBank[i].activity, actRequested))
      h = 1;


  if (idRequested >= tasksUsed || idRequested <= 0) {
    printf("no such task\n");
    val = 0;
  } else if (iqual(actRequested, "TO DO") && !iqual(taskBank[idRequested-1].act, "TO DO")) {
    printf("task already started\n");
    val = 0;
  } else if (!j && val)
    printf("no such user\n");
  else if (!h && val)
    printf("no such activity\n");

  if (j && h && val) { /* If all the arguments are valid...*/ 
    if (iqual(actRequested, "DONE")) { 
      /* If the user wants to move task to "DONE" */
      taskBank[idRequested - 1].started = 0;

      if (iqual(taskBank[idRequested-1].act, "TO DO")) 
        taskBank[idRequested - 1].timePostStart = time;
      
      printf("duration=%d slack=%d\n",
      time - taskBank[idRequested - 1].timePostStart,
      time - taskBank[idRequested - 1].timePostStart 
      - taskBank[idRequested - 1].timeRequested); 
    } else {
      if (iqual(taskBank[idRequested-1].act, "TO DO")){
        taskBank[idRequested-1].timePostStart = time;
      }
      taskBank[idRequested - 1].started = 1;
    }
    strcpy(taskBank[idRequested - 1].act, actRequested);
  }
}

void actList(char arguments[], struct act actBank[ACTMAX]) {
  int i = 0, result = 1, j = 0, h[TASKMAX], * sortedint, count = 0, pote[TASKMAX], printer, * sortedDesc, z;
  char jar[CARMAX];

  while (arguments[i] != '\n') {
    if (i != 0)
      jar[i - 1] = arguments[i];
    ++i;
  }
  jar[i - 1] = '\0';

  for (i=0; i <= ACTMAX; ++i)
    if (iqual(actBank[i].activity, jar))
      result = 0;

  if (result)
    printf("no such activity\n");
  else {
    for (i = 0; i < TASKMAX; ++i)
      if (iqual(jar, taskBank[i].act)) {
        h[j] = i;
        ++j;
      }
    if (j != 0) {
      sortedint = sortElements(j, h, 0);
      for (i = 0; i < j; ++i) { /* dentro do sortedint vamos percorrer cada elemento*/
        pote[0] = sortedint[i];
        z = 1;
        for (count = i + 1; count < j && taskBank[pote[0]].timePostStart == taskBank[sortedint[count]].timePostStart; ++count) {
            pote[z] = sortedint[count];
            ++z;
        }
        i += z-1;
        sortedDesc = sortElements(z+1, pote, 1);
        for (printer = 0; printer < z; ++printer)
            printf("%d %d %s\n", taskBank[sortedDesc[printer]].id,
              taskBank[sortedDesc[printer]].timePostStart, taskBank[sortedDesc[printer]].desc);
      }
    }
  }
}

int addAct(int actUsed, char arguments[], struct act actBank[]) {
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
      if (iqual(jar, actBank[i].activity)) {
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

int * sortElements(int tasksUsed, int * IDs, int isDesc) {
  int jar, i, h, holder;

  for (i = 1; i < tasksUsed-1; ++i) {
    h = i - 1;
    jar = i;
    if (isDesc) {
      while (h >= 0 && less(taskBank[IDs[jar]].desc, taskBank[IDs[h]].desc)) {
        holder = IDs[jar];
        IDs[jar] = IDs[h];
        IDs[h] = holder;
        --jar;
        --h;
      }
    } else {
      while (h >= 0 && taskBank[IDs[jar]].timePostStart < taskBank[IDs[h]].timePostStart) {
        holder = IDs[jar];
        IDs[jar] = IDs[h];
        IDs[h] = holder;
        --jar;
        --h;
      }
    }
  }
  return IDs;
}

/*
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}


int partition(int array[], int low, int high) {
  int pivot = array[high];
  int i = (low - 1);
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    int pi = partition(array, low, high);
    quickSort(array, low, pi - 1);
    quickSort(array, pi + 1, high);
  }
}
*/



int eval(char c) {
  if (c == 't' || c == 'l' || c == 'n' || c == 'u' || c == 'm' || c == 'd' || c == 'a')
    return 1;
  return 0;
}