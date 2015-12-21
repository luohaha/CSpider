#include "scheduler.h"

/**
  watcher : watch the work queues, to find out if there is work to do
  @handle : the uv_idle_t
**/
void watcher(uv_idle_t *handle) {
  cspider_t *cspider = (cspider_t*)handle->data;
  uv_rwlock_wrlock(cspider->lock);
  if (!isTaskQueueEmpty(cspider->task_queue)) {
    /*
      if there is task unhandled yet, start work thread
     */
    if (cspider->download_thread <= cspider->download_thread_max) {
      /*
	when thread's number reach the max limit
       */
      cs_task_queue *rem = removeTask(cspider->task_queue, cspider->task_queue->next->task);
    
      assert(rem != NULL);
    
      uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
      req->data = rem->task;
      //Point to the worker
      ((cs_task_t*)rem->task)->worker = req;
      ((cs_task_t*)rem->task)->cspider = cspider;
      addTask(cspider->task_queue_doing, rem);
      uv_queue_work(cspider->loop, req, download, work_done);

      //add thread's number
      cspider->download_thread++;
    }
  }
  
  if (!isDataQueueEmpty(cspider->data_queue)) {
    //if there is data required to be processed,
    // start thread
    if (cspider->pipeline_thread <= cspider->pipeline_thread_max) {
      cs_rawText_queue *rem = removeData(cspider->data_queue, cspider->data_queue->next->data);
      assert(rem != NULL);
      uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));
      
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
    uv_rwlock_wrunlock(cspider->lock);
    uv_idle_stop(handle);
  }
}
