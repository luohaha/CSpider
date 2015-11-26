#include "downloader.h"

uv_loop_t *loop;
uv_idle_t *idler;
/*
存放数据池的队列
*/
cs_rawText_queue *data_queue;
/*
存放未执行的任务的队列
*/
cs_task_queue *task_queue;
/*
存放正在执行的任务的队列
*/
cs_task_queue *task_queue_doing;

size_t save_data(void *ptr, size_t size, size_t nmemb, char **save) {
  size_t current = strlen(*save);
  size_t all = size * nmemb;
  *save = (char*)realloc(*save, all+current);
  strcpy((*save)+current, (char*)ptr);
  return all;
}

void download(uv_work_t *req) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  char *tmp = (char*)malloc(sizeof(char));
  ((cs_task_t*)(req->data))->data = &tmp;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ((cs_task_t*)(req->data))->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmp);
    
    res = curl_easy_perform(curl);
    printf("%p\n", ((cs_task_t*)(req->data))->data);
    //curl_easy_cleanup(curl);
  }
}

/*
  work线程完成工作后
*/
void work_done(uv_work_t *req, int status) {
  //cs_task_queue *q = removeTask(task_queue_doing, req->data);
  //assert(q != NULL);
  printf("%s\n", *((cs_task_t*)(req->data))->data);
  //freeTask(q);
  return;
}

void watcher(uv_idle_t *handle) {
  if (!isTaskQueueEmpty(task_queue)) {
    //还有未执行或未完成的任务
    //启动work线程
    cs_task_queue *rem = removeTask(task_queue, task_queue->next->task);
    
    assert(rem != NULL);
    
    uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
    req->data = rem->task;
    //指向执行自己的工作线程handle
    rem->task->worker = req;
    addTask(task_queue_doing, rem);
    uv_queue_work(loop, req, download, work_done);
  } else {
    uv_idle_stop(handle);
  }
}

int main(int argc, char **argv) {
  loop = uv_default_loop();
  idler = (uv_idle_t*)malloc(sizeof(uv_idle_t));
  
  data_queue = initDataQueue();
  task_queue = initTaskQueue();
  task_queue_doing = initTaskQueue();

  createTask(task_queue, "www.baidu.com", NULL, 1);
  //createTask(task_queue, "www.youku.com", NULL, 1);

  uv_idle_init(loop, idler);
  uv_idle_start(idler, watcher);
  
  return uv_run(loop, UV_RUN_DEFAULT);
}
