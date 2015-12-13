#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "CS.h"

/*
  任务
  存放要执行抓取的url
*/
typedef struct cs_task_struct {
  char *url;//存放的url
  cs_rawText_t *data;//指向任务完成后的数据
  uv_work_t *worker;//指向执行此任务的工作线程的handle
  cspider_t *cspider;//指向当前拥有此任务的spider
} cs_task_t;

/*
  任务队列
*/
struct cs_task_queue_struct {
  cs_task_t *task;
  struct cs_task_queue_struct *next;
  struct cs_task_queue_struct *prev;
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
