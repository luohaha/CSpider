#ifndef CSPIDER_H
#define CSPIDER_H

#include "CS.h"
#include "downloader.h"
#include "pageProcesser.h"
#include "utils.h"

#define DOWNLOAD 1
#define SAVE     0
#define POST 1
#define GET 0

typedef struct site_struct {
  char *user_agent;//主机信息
  char *proxy;//代理地址
  char *cookie;//存放cookie
  long timeout;//超时时间
} site_t;

struct cspider_struct {
  uv_loop_t *loop;
  uv_idle_t *idler;
  //任务队列
  cs_task_queue *task_queue_doing;
  cs_task_queue *task_queue;
  //获取到的数据队列
  cs_rawText_queue *data_queue;
  cs_rawText_queue *data_queue_doing;
  //提供给用户的函数接口
  void (*process)(struct cspider_struct *cspider, char *d);
  void (*save)(void *data);
  //最大线程数量，和当前的线程数量
  int download_thread_max;
  int pipeline_thread_max;
  int download_thread;
  int pipeline_thread;
  //锁
  uv_rwlock_t *lock;
  //机器信息
  site_t *site;
};

cspider_t *init_cspider();
void cs_setopt_url(cspider_t *cspider, char *url);
void cs_setopt_cookie(cspider_t *cspider, char *cookie);
void cs_setopt_useragent(cspider_t *cspider, char *agent);
void cs_setopt_proxy(cspider_t *cspider, char *proxy);
void cs_setopt_timeout(cspider_t *cspider, long timeout);
void cs_setopt_process(cspider_t *cspider, void (*process)(cspider_t *, char*));
void cs_setopt_save(cspider_t *cspider, void (*save)(void*));
void cs_setopt_threadnum(cspider_t *cspider, int flag, int number);
int cs_run(cspider_t *cspider);

#endif
