#ifndef CS_H
#define CS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<uv.h>
#include<assert.h>
#include<inttypes.h>
#include<curl/curl.h>
#include<pcre.h>

/*
保存返回的数据块的最大数目
*/
#define BUFFER_MAX_NUMBER 1024

#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif

typedef struct cspider_struct cspider_t;

typedef struct cs_task_queue_struct cs_task_queue;
typedef struct cs_rawText_queue_struct cs_rawText_queue;
typedef struct cs_rawText_struct cs_rawText_t;

void watcher(uv_idle_t *handle);
size_t save_data(void *ptr, size_t size, size_t nmemb, void *save);
void download(uv_work_t *req);
void work_done(uv_work_t *req, int status);

//extern uv_loop_t *loop;
//extern uv_idle_t *idler;
/*
存放数据池的队列
*/
//extern cs_rawText_queue *data_queue;
/*
存放未执行的任务的队列
*/
//extern cs_task_queue *task_queue;
/*
存放正在执行的任务的队列
*/
//extern cs_task_queue *task_queue_doing;

#endif
