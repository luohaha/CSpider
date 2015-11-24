#include "downloader.h"

uv_loop_t *loop;
uv_async_t *async;
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

void on_exit(uv_process_t *req, int64_t exit_status, int signal) {
  fprintf(stderr, "process exit with status %lld, signal %d\n", exit_status, signal);
  uv_close((uv_handle_t*)req, NULL);
}

size_t save_data(void *ptr, size_t size, size_t nmemb, char **save) {
  size_t current = strlen(*save);
  size_t all = size * nmemb;
  *save = (char*)realloc(*save, all+current);
  strcpy((*save)+current, (char*)ptr);
  return all;
}

void download(uv_work_t *req) {
  
  char *buf = (char*)malloc(sizeof(char));
  createDataAndInsert(data_queue, &buf, "html");
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, req->data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    
    res = curl_easy_perform(curl);
    
    uv_async_send(async);

    curl_easy_cleanup(curl);
  }
}

void print_data(uv_async_t *handle) {
  printf("%s\n", *data_queue->next->data->data);
  printf("下载完成\n");
  uv_close((uv_handle_t*)handle, NULL);
}

void watcher(uv_idle_t *handle) {
  if (task_queue->next != task_queue || task_queue_doing->next != task_queue_doing) {
    //还有未执行或未完成的任务
    //启动work线程
    uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
    req->data = task_queue->next->task->url;
    uv_queue_work(loop, req, download, NULL);
  } else {
    uv_idle_stop(handle);
  }
}

int main(int argc, char **argv) {
  loop = uv_default_loop();
  idler = (uv_idle_t*)malloc(sizeof(uv_idle_t));
  async = (uv_async_t*)malloc(sizeof(uv_async_t));
  data_queue = initDataQueue();
  task_queue = initTaskQueue();
  task_queue_doing = initTaskQueue();

  insertIntoTaskQueue(task_queue, "www.baidu.com", NULL, 1);
  insertIntoTaskQueue(task_queue, "www.youku.com", NULL, 1);

  uv_idle_start(idler, watcher);

  uv_async_init(loop, async, print_data);
  
  return uv_run(loop, UV_RUN_DEFAULT);
}
