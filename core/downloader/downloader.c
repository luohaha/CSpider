#include "downloader.h"
#include "spider.h"
#include "pageProcesser.h"


/**
  execute after curl get data.
  @ptr : point to string which curl get
  @size :
  @nmemb : @size * @nmemb equal the size of string
  @ss : input pointer
**/
size_t save_data(void *ptr, size_t size, size_t nmemb, void *ss) {
  cs_task_t *save = (cs_task_t*)ss;
  
  size_t all = size * nmemb;
  save->data->data[save->data->count] = (char*)malloc(sizeof(char)*all);
  strncpy(save->data->data[save->data->count], (char*)ptr, all);
  save->data->each[save->data->count] = all;
  save->data->count++;
  save->data->length += all;
  return all;
}

/**
  use curl to download
  @req : the worker
**/
void download(uv_work_t *req) {
  CURL *curl;
  CURLcode res;
  
  cs_task_t *task = (cs_task_t*)req->data;
  cspider_t *cspider = task->cspider;
  site_t *site = (site_t*)cspider->site;
  curl = curl_easy_init();
  
  if (curl) {
    if (site->user_agent != NULL) {
      curl_easy_setopt(curl, CURLOPT_USERAGENT, site->user_agent);
    }
    if (site->proxy != NULL) {
      curl_easy_setopt(curl, CURLOPT_PROXY, site->proxy);
    }
    if (site->timeout != 0) {
      curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, site->timeout);
    }
    if (site->cookie != NULL) {
      curl_easy_setopt(curl, CURLOPT_COOKIE, site->cookie);
    }
    /*支持重定向*/
    /*support redirection*/
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    
    curl_easy_setopt(curl, CURLOPT_URL, task->url);
    /*
      put url to cs_rawText_t
     */
    task->data->url = task->url;
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, req->data);
    res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
  }
}

/**
  work_done : it will be called after work thread finish
  @req : the worker
**/
void work_done(uv_work_t *req, int status) {
  cspider_t *cspider = ((cs_task_t*)req->data)->cspider;
  /*打印到日志
    print log
   */
  if (cspider->log != NULL) {
    uv_rwlock_wrlock(cspider->log_lock);
    fprintf(cspider->log, "download finish : %s\n", ((cs_task_t*)req->data)->url);
    uv_rwlock_wrunlock(cspider->log_lock);
  }
  /*
    when finish download data, 
    first, remove task from task_queue_doing 
    second, add rawText to data_queue
    finally, free the task.
   */
  uv_rwlock_wrlock(cspider->lock);
  cspider->download_thread--;
  cs_task_queue *q = removeTask(cspider->task_queue_doing, req->data);
  assert(q != NULL);
  
  cs_rawText_queue *queue = (cs_rawText_queue*)malloc(sizeof(cs_rawText_queue));
  queue->data = q->task->data;
  addData(cspider->data_queue, queue);
  freeTask(q);
  uv_rwlock_wrunlock(cspider->lock);
  return;
}



/*
int main(int argc, char **argv) {
  
}
*/
