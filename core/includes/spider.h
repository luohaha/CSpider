#ifndef CSPIDER_H
#define CSPIDER_H

#include "CS.h"
#include "downloader.h"

#define DOWNLOAD 1
#define SAVE     0

struct cspider_struct {
  uv_loop_t *loop;
  uv_idle_t *idler;
  cs_task_queue *task_queue_doing;
  cs_task_queue *task_queue;
  cs_rawText_queue *data_queue;
  cs_rawText_queue *data_queue_doing;
  void (*process)(struct cspider_struct *cspider, char *d);
  void (*save)(char *str);
  int download_thread_max;
  int pipeline_thread_max;
  int download_thread;
  int pipeline_thread;
};

cspider_t *init_cspider();
void cs_setopt_url(cspider_t *cspider, char *url, request_t *param, int prior);
void cs_setopt_process(cspider_t *cspider, void (*process)(cspider_t *, char*));
void cs_setopt_save(cspider_t *cspider, void (*save)(char*));
void cs_setopt_threadnum(cspider_t *cspider, int flag, int number);
int cs_run(cspider_t *cspider);

#endif
