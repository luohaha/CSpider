#include "downloader.h"
#include "pageProcesser.h"

/**
isTaskQueueEmpty : is task queue empty?
@head : task queue ready to test
return 1 for empty, 0 for no empty
**/
int isTaskQueueEmpty(cs_task_queue *head) {
  return (head->next == head);
}
/**

initTaskQueue : return the initializing queue

return the new task queue.
**/
cs_task_queue *initTaskQueue() {
  cs_task_queue *new_queue = (cs_task_queue*)malloc(sizeof(cs_task_queue));
  PANIC(new_queue);
  
  new_queue->task = NULL;
  new_queue->next = new_queue;
  new_queue->prev = new_queue;
  return new_queue;
}
/**
createTask : add a task to task queue
@head : the task queue ready to be inserted
@url : new task's url
**/
void createTask(cs_task_queue *head, char *url) {
  cs_task_t *task = (cs_task_t*)malloc(sizeof(cs_task_t));
  PANIC(task);
  
  task->url = url;
  /*需要先新建一个存放数据的地方*/
  task->data = createData();
  cs_task_queue *queue = (cs_task_queue*)malloc(sizeof(cs_task_queue));
  PANIC(queue);
  
  queue->task = task;
  queue->next = head;
  queue->prev = head->prev;
  queue->next->prev = queue;
  queue->prev->next = queue;
  return;
}
/**

removeTask : remove the task from the task queue
@head : the task queue ready to be removed from
@task : removed task

if @task exists in @head, 
return @task
else
return NULL
 
**/
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
/**
  
  addTask : add a task into queue
  @head : the task queue ready to be inserted
  @task : the task ready to insert into @head

**/
void addTask(cs_task_queue *head, cs_task_queue *task) {
  task->next = head;
  task->prev = head->prev;
  task->next->prev = task;
  task->prev->next = task;
  return;
}
/**
freeTask : free the task queue
@node : the task ready to be freed

first, free the uv_work_t
second, free cs_task_t
finally, frr cs_task_queue

**/
void freeTask(cs_task_queue *node) {
  free(node->task->worker);
  free(node->task);
  free(node);
}
