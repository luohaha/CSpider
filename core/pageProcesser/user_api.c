#include "pageProcesser.h"

/*
  provide two functions to users.
  saveString : save the string.
  addUrl : add the url back to the download task queue.
*/
/**
  saveString : api of data persistence
  @cspider : the cspider_t
  @data : the pointer point to data which is ready to be saved
**/
void saveString(cspider_t *cspider, void *data) {
  uv_rwlock_wrlock(cspider->save_lock);
  (cspider->save)(data, cspider->save_user_data);
  uv_rwlock_wrunlock(cspider->save_lock);
}
/**
  addUrl : add url back to task queue
  @cspider : the cspider_t
  @url : url added back to task queue
**/
void addUrl(cspider_t *cspider, char *url) {
  unsigned int len = strlen(url);
  char *reUrl = (char*)malloc(sizeof(char) * len);
  strncpy(reUrl, url, len);
  uv_rwlock_wrlock(cspider->lock);
  createTask(cspider->task_queue, reUrl);
  uv_rwlock_wrunlock(cspider->lock);
}
/**
  addUrls : add many urls back to task queue
  @cspider : the cspider_t
  @urls : the array of url
  @size : the size of @urls
**/
void addUrls(cspider_t *cspider, char **urls, int size) {
  int i;
  char *reUrls[size];
  for (i = 0; i < size; i++) {
    unsigned int len = strlen(urls[i]);
    reUrls[i] = (char*)malloc(sizeof(char) * len);
    strncpy(reUrls[i], urls[i], len);
  }
  uv_rwlock_wrlock(cspider->lock);
  for (i = 0; i < size; i++) {
    createTask(cspider->task_queue, reUrls[i]);
  }
  uv_rwlock_wrunlock(cspider->lock);
}
