#ifndef CSPIDER_H
#define CSPIDER_H

#include "CS.h"

#define DOWNLOAD 1
#define SAVE     0

struct cspider_struct {
  uv_loop_t *loop;
  uv_idle_t *idler;
  cs_task_queue *task_queue_doing;
  cs_task_queue *task_queue;
  cs_rawText_queue *data_queue;
  void (*process)(cs_rawText_t *data);
  void (*save)(char *str);
  int download_thread_max;
  int pipeline_thread_max;
  int download_thread;
  int pipeline_thread;
};

#endif
