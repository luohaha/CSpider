#include "pageProcesser.h"

/**
  dataproc : process data
  @req : the worker

  call this function to process the string we get
**/
void dataproc(uv_work_t *req) {
  cspider_t *cspider = ((cs_rawText_t*)req->data)->cspider;
  cs_rawText_t *text = (cs_rawText_t*)req->data;
  //Put all buffer's data into a string
  char *get = (char*)malloc(sizeof(char) * text->length+1);
  PANIC(get);
  
  int i;
  int currentCount = 0;
  for (i = 0; i < text->count; i++) {
    strncpy(get+currentCount, text->data[i], text->each[i]);
    currentCount += text->each[i];
  }
  *(get+currentCount) = '\0';
  //get data
  (cspider->process)(cspider, get, text->url, cspider->process_user_data);
  free(get);
}

/**
   datasave : call after data process worker finish
   @req : the worker

 **/
void datasave(uv_work_t *req, int status) {
  cspider_t *cspider = ((cs_rawText_t*)req->data)->cspider;
  //log
  logger(0, "%s save finish.\n", ((cs_rawText_t*)req->data)->url, cspider);
  
  uv_rwlock_wrlock(cspider->lock);
  cspider->pipeline_thread--;
  cs_rawText_queue *q = removeData(cspider->data_queue_doing, req->data);
  PANIC(q);
  
  logger(q != NULL, "removeData error in %s.\n", "dataProcess.c", cspider);
  freeData(q);
  uv_rwlock_wrunlock(cspider->lock);
}

