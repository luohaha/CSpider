#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "CS.h"
#include "downloader.h"
#include "pageProcesser.h"
#include "spider.h"

int watcher(uv_idle_t *handle);

#endif
