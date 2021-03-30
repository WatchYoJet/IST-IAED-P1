#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <string.h>

#include "eval.c"

/*---------------------------Defines---------------------------------*/

#define DESCMAX 50
#define USERMAX 20
#define CARMAX 20
#define ACTMAX 10
#define MAXINPUT 500
#define TASKMAX 10000
#define DIFFUSERMAX 50

struct task {
  int started;
  int id; /*O ID da tarefa, int de 1 a 10000 */
  char desc[DESCMAX]; /*A descrição da tarefa, string de 50 caracteres com espaços*/
  char user[USERMAX]; /*O utilizador da tarefa, string de 20 caracteres sem espaços */
  char act[CARMAX]; /*A atividade na qual a tarefa está. String de 20 caracters com espaços */
  int timeRequested; /*O tempo estimado para a atividade ser completada */
  int t0; /*O instante no qual a ativdade foi começada */
};

struct user {
  char username[USERMAX];
};

struct act {
  char activity[CARMAX];
};

/*---------------------------Functions-------------------------------*/

int eval(char c);
int addTask(int id, char arguments[], struct task taskBank[]);
void taskList(int id, struct task taskBank[], char arguments[]);
int timeForward(char arguments[], int time, int id, struct task taskBank[]);
int addUser(char arguments[], struct user userBank[], int userUsed);
void moveTask(int tasksUsed, struct task taskBank[TASKMAX], struct user userBank[], char arguments[], struct act actBank[]);
void actList(char arguments[], struct act actBank[]);
int addAct(int actUsed, char arguments[], struct act actBank[]);

/*----------------------------Main-----------------------------------*/

int main() {
  char command, arguments[MAXINPUT];
  int tasksUsed = 1, time = 0, userUsed = 0, actUsed = 3;
  struct task taskBank[TASKMAX];
  struct user userBank[DIFFUSERMAX];
  struct act actBank[ACTMAX];

  strcpy(actBank[0].activity,"TO DO");
  strcpy(actBank[1].activity,"IN PROGRESS");
  strcpy(actBank[2].activity,"DONE");

  while ((command = getchar()) != 'q') {
    fgets(arguments, MAXINPUT + 2, stdin);
    if (eval(command))
      switch (command) {
      case 't':
        tasksUsed = addTask(tasksUsed, arguments, taskBank);
        break;
      case 'l':
        taskList(tasksUsed, taskBank, arguments);
        break;
      case 'n':
        time = timeForward(arguments, time, tasksUsed, taskBank);
        break;
      case 'u':
        userUsed = addUser(arguments, userBank, userUsed);
        break;
      case 'm':
        moveTask(tasksUsed, taskBank, userBank, arguments, actBank);
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

int addTask(int id, char arguments[], struct task taskBank[]) {
  char descript[DESCMAX];
  int time, i, val = 0;

  sscanf(arguments, "%d %[^\n]", & time, descript);
  for (i = 0; i < id; ++i)
    if (!strcmp(taskBank[i].desc, descript))
      val = 1;
  if (val) {
    printf("duplicate description\n");
    return id;
  } else if (id > TASKMAX) {
    printf("too many tasks\n");
    return id;
  } else {
    taskBank[id - 1].id = id;
    taskBank[id - 1].timeRequested = time;
    strcpy(taskBank[id - 1].act, "TO DO");
    strcpy(taskBank[id - 1].desc, descript);
    printf("task %d %s\n", id, descript);
  }
  return ++id;
}

void taskList(int id, struct task taskBank[], char arguments[]) {
  int idCounter, i = 0, inputid, j, reset;
  char jar[MAXINPUT], numstr[5], pote[5];

  strcpy(jar, arguments);

  if (jar[0] == '\n')
    for (idCounter = 0; idCounter < id - 1; ++idCounter)
      printf("%d %s #%d %s\n", taskBank[idCounter].id, taskBank[idCounter].act,
        taskBank[idCounter].timeRequested, taskBank[idCounter].desc);
  else {
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
        if (!taskBank[inputid].id)
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

int timeForward(char arguments[], int time, int id, struct task taskBank[]) {
  int tint, i;

  sscanf(arguments, "%d", & tint);
  if (tint >= 0) {
    printf("%d\n", time += tint);
    for (i = 0; i < id; ++i)
      if (taskBank[i].started)
        taskBank[i].t0 += tint;
  } else
    printf("invalid time\n");
  return time;
}

int addUser(char arguments[], struct user userBank[], int userUsed) {
  int i;
  char username[USERMAX];

  sscanf(arguments, "%s", username);
  for (i = 0; i < DIFFUSERMAX; ++i)
    if (!strcmp(userBank[i].username, username)) {
      printf("user already exists\n");
      return userUsed;
    }
  if (userUsed == DIFFUSERMAX) {
    printf("too many users\n");
    return userUsed;
  }
  strcpy(userBank[userUsed].username, username);
  if (userUsed != 0)
    for (i = 0; i <= userUsed; ++i)
      printf("%s\n", userBank[i].username);
  return ++userUsed;
}

void moveTask(int tasksUsed, struct task taskBank[TASKMAX], struct user userBank[],
               char arguments[], struct act actBank[]){

  int idRequested, val=1, i,j=0, h=0;
  char userRequested[USERMAX], actRequested[CARMAX];

  sscanf(arguments, "%d %s %[^\n]", &idRequested, userRequested, actRequested);

  for(i=0; i < DIFFUSERMAX; ++i)
    if (!strcmp(userBank[i].username, userRequested))
      ++j;
  if (!j)
    printf("no such user\n");
  for(i=0; i < ACTMAX; ++i)
    if (!strcmp(actBank[i].activity, actRequested))
      ++h;
  if (!h)
    printf("no such activity\n");

  if (idRequested >= tasksUsed){
    printf("no such task\n");
    val = 0;
  }
  else if (!strcmp(actRequested,"TO DO")){
    printf("task already started\n");
    val = 0;
  }

  if (j && h && val){
    if (!strcmp(actRequested, "DONE")){
      printf("duration=%d slack=%d\n", taskBank[idRequested-1].t0, 
                                taskBank[idRequested-1].t0-taskBank[idRequested-1].timeRequested);
      strcpy(taskBank[idRequested-1].act, "DONE");
    }
    else{
      strcpy(taskBank[idRequested-1].act, actRequested);
      taskBank[idRequested-1].started = 1;
    }
  }
}


void actList(char arguments[], struct act actBank[]) {
  int i = 0, result = 1;
  char jar[CARMAX];

  while (arguments[i] != '\n') {
    if (i != 0)
      jar[i - 1] = arguments[i];
    ++i;
  }
  jar[i-1] = '\0';

  for (i = 0; i < CARMAX; ++i)
    if (!strcmp(jar, actBank[i].activity)) {
      printf("%s\n", actBank[i].activity);
      result = 0;
    }
  if (result)
    printf("no such activity\n");
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
    jar[i] = '\0';
    i = 0;
    while (jar[i] != '\0') {
      if (!((jar[i] >= 'A' && jar[i] <= 'Z') || jar[i] == ' ' || jar[i] == '\t')) {
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
  } else
    for (i = 0; i < actUsed; ++i)
      printf("%s\n", actBank[i].activity);
  return ++actUsed;
}