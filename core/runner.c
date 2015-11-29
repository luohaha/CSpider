#include "spider.h"
#include "downloader.h"

//uv_loop_t *loop;
//uv_idle_t *idler;
/*
存放数据池的队列
*/
//cs_rawText_queue *data_queue;
/*
存放未执行的任务的队列
*/
//cs_task_queue *task_queue;
/*
存放正在执行的任务的队列
*/
//cs_task_queue *task_queue_doing;

cspider_t *init_cspider() {
  cspider_t *spider = (cspider_t *)malloc(sizeof(cspider_t));
  spider->task_queue = initTaskQueue();
  spider->download_thread_max = 5;
  spider->pipeline_thread_max = 5;
  spider->download_thread = 1;
  spider->pipeline_thread = 1;
  spider->process = NULL;
  spider->save = NULL;
  spider->loop = uv_default_loop();
  spider->task_queue_doing = initTaskQueue();
  spider->data_queue = initDataQueue();
  spider->data_queue_doing = initDataQueue();
  spider->idler = (uv_idle_t*)malloc(sizeof(uv_idle_t));
  spider->lock = (uv_rwlock_t*)malloc(sizeof(uv_rwlock_t));
  uv_rwlock_init(spider->lock);
  spider->idler->data = spider;
  return spider;
}

void cs_setopt_url(cspider_t *cspider, char *url, request_t *param, int prior) {
  assert(cspider != NULL);
  assert(url != NULL);
  assert(prior >= 1 && prior <= 10);
  createTask(cspider->task_queue, url, param, prior);
}

void cs_setopt_process(cspider_t *cspider, void (*process)(cspider_t *, char*)) {
  assert(cspider != NULL);
  cspider->process = process;
}

void cs_setopt_save(cspider_t *cspider, void (*save)(char*)) {
  assert(cspider != NULL);
  cspider->save = save;
}

void cs_setopt_threadnum(cspider_t *cspider, int flag, int number) {
  assert(flag == DOWNLOAD || flag == SAVE);
  if (flag == DOWNLOAD) {
    cspider->download_thread_max = number;
  } else {
    cspider->pipeline_thread_max = number;
  }
}

int cs_run(cspider_t *cspider) {
  
  uv_idle_init(cspider->loop, cspider->idler);
  uv_idle_start(cspider->idler, watcher);
  
  return uv_run(cspider->loop, UV_RUN_DEFAULT);
}
