#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "CS.h"

/*
  任务
  存放要执行抓取的url

  the struct of the task
*/
typedef struct cs_task_struct {
  char *url;//the url which task need to deal with
  cs_rawText_t *data;//Point to the struct which save the data we download.
  uv_work_t *worker;//Point to the worker
  cspider_t *cspider;//Point to the Main cspider struct
} cs_task_t;

/*
  任务队列

  task queue
*/
struct cs_task_queue_struct {
  cs_task_t *task; // 
  struct cs_task_queue_struct *next; // next node
  struct cs_task_queue_struct *prev; // previous node
};


/*task.c*/
int isTaskQueueEmpty(cs_task_queue *head);
cs_task_queue *initTaskQueue();
void createTask(cs_task_queue *head, char *url);
cs_task_queue *removeTask(cs_task_queue *head, cs_task_t *task);
void addTask(cs_task_queue *head, cs_task_queue *task);
void freeTask(cs_task_queue *node);


//extern cs_rawText_queue *data_queue;
#endif
