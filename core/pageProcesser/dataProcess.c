#include "pageProcesser.h"

void dataproc(uv_work_t *req) {
  cspider_t *cspider = ((cs_rawText_t*)req->data)->cspider;
  cs_rawText_t *text = (cs_rawText_t*)req->data;
  //将buffer链表中的数据取出，统一存放在一个字符串中
  char *get = (char*)malloc(sizeof(char) * text->length+1);
  assert(get != NULL);
  int i;
  int currentCount = 0;
  for (i = 0; i < text->count; i++) {
    strncpy(get+currentCount, text->data[i], text->each[i]);
    currentCount += text->each[i];
  }
  *(get+currentCount) = '\0';
  //得到了数据get
  (cspider->process)(cspider, get);
  free(get);
}

void datasave(uv_work_t *req) {
  cspider_t *cspider = ((cs_rawText_t*)req->data)->cspider;
  uv_rwlock_wrlock(cspider->lock);
  cspider->pipeline_thread--;
  cs_rawText_queue *q = removeData(cspider->data_queue_doing, req->data);
  assert(q != NULL);
  freeData(q);
  uv_rwlock_wrunlock(cspider->lock);
}

/*
  数据持久化的接口
*/
void saveString(cspider_t *cspider, char *data) {
  (cspider->save)(data);
}
/*
  将url加入任务队列的接口
*/
void addUrl(cspider_t *cspider, char *url, int prior) {
  uv_rwlock_wrlock(cspider->lock);
  createTask(cspider->task_queue, url, prior);
  uv_rwlock_wrunlock(cspider->lock);
}
