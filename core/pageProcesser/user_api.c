#include "pageProcesser.h"

/*
  provide two functions to users.
  saveString : save the string.
  addUrl : add the url back to the download task queue.
*/
/*
  数据持久化的接口
*/
void saveString(cspider_t *cspider, void *data) {
  uv_rwlock_wrlock(cspider->save_lock);
  (cspider->save)(data, cspider->save_user_data);
  uv_rwlock_wrunlock(cspider->save_lock);
}
/*
  将url加入任务队列的接口
*/
void addUrl(cspider_t *cspider, char *url) {
  uv_rwlock_wrlock(cspider->lock);
  createTask(cspider->task_queue, url);
  uv_rwlock_wrunlock(cspider->lock);
}
