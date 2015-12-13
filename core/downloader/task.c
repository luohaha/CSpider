#include "downloader.h"
#include "pageProcesser.h"

/*
判断任务队列是否为空
*/
int isTaskQueueEmpty(cs_task_queue *head) {
  return (head->next == head);
}
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
void createTask(cs_task_queue *head, char *url) {
  cs_task_t *task = (cs_task_t*)malloc(sizeof(cs_task_t));
  task->url = url;
  /*需要先新建一个存放数据的地方*/
  task->data = createData();
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
cs_task_queue *removeTask(cs_task_queue *head, cs_task_t *task) {
  cs_task_queue *p = head->next;
  cs_task_queue *res = NULL;
  
  while (p != head) {
    if (p->task == task) {
      res = p;
      p->prev->next = p->next;
      p->next->prev = p->prev;
      break;
    } else {
      p = p->next;
    }
  }
  return res;
}
/*
  将已有任务加入队列
*/
void addTask(cs_task_queue *head, cs_task_queue *task) {
  task->next = head;
  task->prev = head->prev;
  task->next->prev = task;
  task->prev->next = task;
  return;
}
/*
回收任务队列节点
*/
void freeTask(cs_task_queue *node) {
  free(node->task->worker);
  free(node->task);
  free(node);
}
