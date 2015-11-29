#include "pageProcesser.h"

void dataproc(uv_work_t *req) {
  cspider_t *cspider = ((cs_rawText_t*)req->data)->cspider;
  (cspider->process)(cspider, ((cs_rawText_t*)req->data)->data);
  //(cspider->process)(cspider, "ttd");
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
void addUrl(cspider_t *cspider, char *url, request_t *req, int prior) {
  uv_rwlock_wrlock(cspider->lock);
  createTask(cspider->task_queue, url, req, prior);
  uv_rwlock_wrunlock(cspider->lock);
}
