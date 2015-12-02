#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "CS.h"


/*
key value对
*/
typedef struct param_struct {
  char *key;
  char *value;
} param_t;
/*
  请求类型:post get
*/
typedef struct request_struct {
  char *type;
  param_t *param;
} request_t;
/*
  任务
  存放要执行抓取的url
*/
typedef struct cs_task_struct {
  char *url;//存放的url
  request_t *req;
  int prior;//优先级1~10
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

/*
  原始数据
  例如html，json等，直接返回的数据
*/
struct cs_rawText_struct {
  char *data[BUFFER_MAX_NUMBER];
  int count;//数据块的个数
  int length;//数据的长度
  char *type;//数据的类型，比如html，json
  uv_work_t *worker;//指向执行此任务的工作线程的handle
  cspider_t *cspider;//指向当前拥有此任务的spider
};

/*
  数据队列
*/
struct cs_rawText_queue_struct {
  cs_rawText_t *data;
  struct cs_rawText_queue_struct *next;
  struct cs_rawText_queue_struct *prev;
};

/*data.c*/
int isDataQueueEmpty(cs_rawText_queue *head);
cs_rawText_queue *initDataQueue();
cs_rawText_t *createData(const char* type);
void addData(cs_rawText_queue *head, cs_rawText_queue *queue);
cs_rawText_queue *removeData(cs_rawText_queue *head, cs_rawText_t *data);
void freeData(cs_rawText_queue *node);

/*task.c*/
int isTaskQueueEmpty(cs_task_queue *head);
cs_task_queue *initTaskQueue();
void createTask(cs_task_queue *head, char *url, request_t *req, int prior);
cs_task_queue *removeTask(cs_task_queue *head, cs_task_t *task);
void addTask(cs_task_queue *head, cs_task_queue *task);
void freeTask(cs_task_queue *node);


//extern cs_rawText_queue *data_queue;
#endif
