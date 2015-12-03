#ifndef PAGEPROCESSER_H
#define PAGEPROCESSER_H

#include "CS.h"
#include "spider.h"
#include "downloader.h"



void dataproc(uv_work_t *req);
void datasave(uv_work_t *req);
void saveString(cspider_t *cspider, char *data);
void addUrl(cspider_t *cspider, char *url, char *cookie, int prior);

#endif
