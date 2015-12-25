#include "spider.h"
#include "downloader.h"
#include "utils.h"
#include "scheduler.h"

/**
  init_cspider : init the cspider

  return the cspider_t which is ready
**/

cspider_t *init_cspider() {
  cspider_t *spider = (cspider_t *)malloc(sizeof(cspider_t));
  spider->task_queue = initTaskQueue();
  spider->download_thread_max = 5;
  spider->pipeline_thread_max = 5;
  spider->download_thread = 1;
  spider->pipeline_thread = 1;
  spider->process = NULL;
  spider->save = NULL;
  spider->process_user_data = NULL;
  spider->save_user_data = NULL;
  spider->loop = uv_default_loop();
  spider->task_queue_doing = initTaskQueue();
  spider->data_queue = initDataQueue();
  spider->data_queue_doing = initDataQueue();
  spider->idler = (uv_idle_t*)malloc(sizeof(uv_idle_t));
  spider->lock = (uv_rwlock_t*)malloc(sizeof(uv_rwlock_t));
  uv_rwlock_init(spider->lock);
  spider->save_lock = (uv_rwlock_t*)malloc(sizeof(uv_rwlock_t));
  uv_rwlock_init(spider->save_lock);
  spider->idler->data = spider;
  spider->site = (site_t*)malloc(sizeof(site_t));
  spider->site->user_agent = NULL;
  spider->site->proxy = NULL;
  spider->site->cookie = NULL;
  spider->site->timeout = 0;
  spider->log = NULL;
  spider->bloom = init_Bloom();
  return spider;
}
/**
 cs_setopt_url : add urls to task queue
 @cspider : the cspider
 @url : new task's url
**/
void cs_setopt_url(cspider_t *cspider, char *url){
  assert(cspider != NULL);
  assert(url != NULL);
  if (!bloom_check(cspider->bloom, url)) {
    //url no exits
    bloom_add(cspider->bloom, url);
    unsigned int len = strlen(url);
    char *reUrl = (char*)malloc(sizeof(char) * (len+1));
    strncpy(reUrl, url, len+1);
    createTask(cspider->task_queue, reUrl);
  }
}

/**
cs_setopt_cookie : set cookie
@cspider : the cspider
@cookie : the cookie string
**/
void cs_setopt_cookie(cspider_t *cspider, char *cookie) {
  ((site_t*)cspider->site)->cookie = cookie;
}

/**
  cs_setopt_useragent : set user agent
**/
void cs_setopt_useragent(cspider_t *cspider, char *agent) {
  ((site_t*)cspider->site)->user_agent = agent;
}

/**
  cs_setopt_proxy : set proxy
**/
void cs_setopt_proxy(cspider_t *cspider, char *proxy) {
  ((site_t*)cspider->site)->proxy = proxy;
}

/**
 cs_setopt_timeout : set timeout(ms)
**/
void cs_setopt_timeout(cspider_t *cspider, long timeout) {
  ((site_t*)cspider->site)->timeout = timeout;
}
/**
  cs_setopt_logfile : set log file
**/
void cs_setopt_logfile(cspider_t *cspider, FILE *log) {
  cspider->log = log;
  cspider->log_lock = (uv_rwlock_t*)malloc(sizeof(uv_rwlock_t));
  uv_rwlock_init(cspider->log_lock);
}
/**
cs_setopt_process : pass the costom process function to cspider
@cspider : the cspider
@process : the Pointer which point to costom process function
@user_data : Pointer to context data which you want to pass to @process
 **/
void cs_setopt_process(cspider_t *cspider, void (*process)(cspider_t *, char *, char*, void*), void *user_data) {
  assert(cspider != NULL);
  cspider->process = process;
  cspider->process_user_data = user_data;
}
/**
cs_setopt_save : pass the costom save function to cspider
@cspider : the cspider
@save : the Pointer which point to costom save function
@user_data : Pointer to context data which you want to pass to @save
 **/
void cs_setopt_save(cspider_t *cspider, void (*save)(void*, void*), void *user_data){
  assert(cspider != NULL);
  cspider->save = save;
  cspider->save_user_data = user_data;
}

/**
  cs_setopt_threadnum : set thread's number
  @cspider : the cspider
  @flag : thread's type
  @number : thread's max number which we want to set

  if @flag equal DOWNLOAD
  @number will be the max number of download thread
  else if @flag equal SAVE
  @number whill be the max number of save thread
**/
void cs_setopt_threadnum(cspider_t *cspider, int flag, int number) {
  assert(flag == DOWNLOAD || flag == SAVE);
  assert(number > 0);
  if (flag == DOWNLOAD) {
    cspider->download_thread_max = number;
  } else {
    cspider->pipeline_thread_max = number;
  }
}
/**
   cs_run : start the cspider
**/
int cs_run(cspider_t *cspider) {
  if (cspider->process == NULL) {
    printf("warn : need to init process function(use cs_setopt_process)\n");
    return 0;
  }
  if (cspider->save == NULL) {
    printf("warn : need to init data persistence function(use cs_setopt_save)\n");
    return 0;
  }
  uv_idle_init(cspider->loop, cspider->idler);
  uv_idle_start(cspider->idler, watcher);
  
  return uv_run(cspider->loop, UV_RUN_DEFAULT);
}
