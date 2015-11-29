#ifndef PAGEPROCESSER_H
#define PAGEPROCESSER_H

#include "CS.h"
#include "spider.h"
#include "downloader.h"

#define REGEX_ALL 0
#define REGEX_NO_ALL 1

char *regex(const char *regex, char *str, int flag);
char **regexAll(const char *regex, char *str, int *num, int flag);

void dataproc(uv_work_t *req);
void datasave(uv_work_t *req);
void saveString(cspider_t *cspider, char *data);
void addUrl(cspider_t *cspider, char *url, request_t *req, int prior);

#endif
