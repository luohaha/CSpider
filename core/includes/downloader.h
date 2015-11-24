#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "./CS.h"
/*
  任务
  存放要执行抓取的url
*/
typedef struct cs_task_struct {
  char *url;//存放的url
  int prior;//优先级1~10
} cs_task_t;

/*
  任务队列
*/
typedef struct cs_task_queue_struct {
  cs_task_t *task;
  struct cs_task_queue_struct *next;
  struct cs_task_queue_Struct *prev;
} cs_task_queue;

/*
  原始数据
  例如html，json等，直接返回的数据
*/
typedef struct cs_rawText_struct {
  char *data;
  char *type;//数据的类型，比如html，json
} cs_rawText_t;

/*
  数据队列
*/
typedef struct cs_rawText_queue_struct {
  cs_rawText_t *data;
  struct cs_rawText_queue_struct *next;
  struct cs_rawText_queue_struct *prev;
} cs_rawText_queue;

cs_rawText_queue *initDataQueue();
void createDataAndInsert(cs_rawText_queue *head, char *begin, const char* type);

#endif
