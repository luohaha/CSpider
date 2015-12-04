#include "downloader.h"
#include "spider.h"
#include "pageProcesser.h"



size_t save_data(void *ptr, size_t size, size_t nmemb, void *ss) {
  cs_task_t *save = (cs_task_t*)ss;
  
  size_t all = size * nmemb;
  save->data->data[save->data->count] = (char*)malloc(sizeof(char)*all*2);
  strncpy(save->data->data[save->data->count], (char*)ptr, all);
  save->data->each[save->data->count] = all;
  save->data->count++;
  save->data->length += all;
  return all;
}

void download(uv_work_t *req) {
  CURL *curl;
  CURLcode res;
  //((cs_task_t*)req->data)->data->data = (char*)malloc(sizeof(char)*5000);
  cs_task_t *task = (cs_task_t*)req->data;
  cspider_t *cspider = task->cspider;
  site_t *site = (site_t*)cspider->site;
  curl = curl_easy_init();
  
  if (curl) {
    if (site->user_agent != "null") {
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0");
    }
    if (site->proxy != "null") {
      curl_easy_setopt(curl, CURLOPT_PROXY, site->proxy);
    }
    if (site->timeout != 0) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, site->timeout);
    }
    
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); 
    curl_easy_setopt(curl, CURLOPT_URL, task->url);
    curl_easy_setopt(curl, CURLOPT_COOKIE, task->cookie);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, req->data);
    res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
  }
}

/*
  work线程完成工作后
*/
void work_done(uv_work_t *req, int status) {
  cspider_t *cspider = ((cs_task_t*)req->data)->cspider;
  uv_rwlock_wrlock(cspider->lock);
  cspider->download_thread--;
  cs_task_queue *q = removeTask(cspider->task_queue_doing, req->data);
  assert(q != NULL);
  //printf("%s\n", (((cs_task_t*)(req->data))->data->data));
  cs_rawText_queue *queue = (cs_rawText_queue*)malloc(sizeof(cs_rawText_queue));
  queue->data = q->task->data;
  addData(cspider->data_queue, queue);
  freeTask(q);
  uv_rwlock_wrunlock(cspider->lock);
  return;
}

void watcher(uv_idle_t *handle) {
  cspider_t *cspider = (cspider_t*)handle->data;
  uv_rwlock_wrlock(cspider->lock);
  if (!isTaskQueueEmpty(cspider->task_queue)) {
    //还有未执行或未完成的任务
    //启动work线程
    if (cspider->download_thread <= cspider->download_thread_max) {
      /*
	当线程数目未达到上限
       */
      cs_task_queue *rem = removeTask(cspider->task_queue, cspider->task_queue->next->task);
    
      assert(rem != NULL);
    
      uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
      req->data = rem->task;
      //指向执行自己的工作线程handle
      ((cs_task_t*)rem->task)->worker = req;
      ((cs_task_t*)rem->task)->cspider = cspider;
      addTask(cspider->task_queue_doing, rem);
      uv_queue_work(cspider->loop, req, download, work_done);

      //数目加1
      cspider->download_thread++;
    }
  }
  
  if (!isTaskQueueEmpty(cspider->data_queue)) {
    //还有未完成的数据处理任务
    if (cspider->pipeline_thread <= cspider->pipeline_thread_max) {
      cs_rawText_queue *rem = removeData(cspider->data_queue, cspider->data_queue->next->data);
      assert(rem != NULL);
      uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
      //req->data = rem->data;
      //指向工作的线程handle
      ((cs_rawText_t*)rem->data)->worker = req;
      ((cs_rawText_t*)rem->data)->cspider = cspider;
      req->data = rem->data;
      
      addData(cspider->data_queue_doing, rem);
      uv_queue_work(cspider->loop, req, dataproc, datasave);
      cspider->pipeline_thread++;
    }
  }

  if (!isTaskQueueEmpty(cspider->task_queue_doing) ||
      !isTaskQueueEmpty(cspider->task_queue) ||
      !isDataQueueEmpty(cspider->data_queue) ||
      !isDataQueueEmpty(cspider->data_queue_doing)) {

    uv_rwlock_wrunlock(cspider->lock);
    
  } else {
    uv_idle_stop(handle);
  }
}

/*
int main(int argc, char **argv) {
  
}
*/
