#ifndef PAGEPROCESSER_H
#define PAGEPROCESSER_H

#include "CS.h"
#include "spider.h"
#include "downloader.h"



void dataproc(uv_work_t *req);
void datasave(uv_work_t *req);
/*
  用户接口
*/
void saveString(cspider_t *cspider, void *data);
void addUrl(cspider_t *cspider, char *url, int prior);

#endif
