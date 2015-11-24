#include "downloader.h"

/*
初始化任务队列
*/
cs_task_queue *initTaskQueue() {
  cs_task_queue *new_queue = (cs_task_queue*)malloc(sizeof(cs_task_queue));
  new_queue->task = NULL;
  new_queue->next = new_queue;
  new_queue->prev = new_queue;
  return new_queue;
}
/*
给任务队列添加任务
*/
void insertIntoTaskQueue(cs_task_queue *head, char *url, request_t *req, int prior) {
  cs_task_t *task = (cs_task_t*)malloc(sizeof(cs_task_t));
  task->req = req;
  task->url = url;
  task->prior = prior;
  cs_task_queue *queue = (cs_task_queue*)malloc(sizeof(cs_task_queue));
  queue->task = task;
  queue->next = head;
  queue->prev = head->prev;
  queue->next->prev = queue;
  queue->prev->next = queue;
  return;
}
/*
将任务移除出队列
*/
int removeTask(cs_task_queue *head, cs_task_t *task) {
  
}
