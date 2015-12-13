#ifndef PAGEPROCESSER_H
#define PAGEPROCESSER_H

#include "CS.h"
#include "spider.h"
#include "downloader.h"

/*
  原始数据
  例如html，json等，直接返回的数据
*/
struct cs_rawText_struct {
  char *data[BUFFER_MAX_NUMBER];
  unsigned int each[BUFFER_MAX_NUMBER];
  int count;//数据块的个数
  int length;//数据的长度
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
cs_rawText_t *createData();
void addData(cs_rawText_queue *head, cs_rawText_queue *queue);
cs_rawText_queue *removeData(cs_rawText_queue *head, cs_rawText_t *data);
void freeData(cs_rawText_queue *node);


void dataproc(uv_work_t *req);
void datasave(uv_work_t *req, int status);
/*
  用户接口
*/
void saveString(cspider_t *cspider, void *data);
void addUrl(cspider_t *cspider, char *url);

#endif
