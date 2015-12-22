#ifndef CS_H
#define CS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<uv.h>
#include<assert.h>
#include<inttypes.h>
#include<math.h>
#include<float.h>
#include<limits.h>
#include<stdarg.h>
#include<ctype.h>
#include<curl/curl.h>
#include<pcre.h>
#include<libxml/tree.h>
#include<libxml/parser.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>
#include<libxml/HTMLparser.h>
#include<libxml/HTMLtree.h>



/*
the max number of the buffers
*/
#define BUFFER_MAX_NUMBER 1024

#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif

typedef struct cspider_struct cspider_t;

typedef struct cs_task_queue_struct cs_task_queue;
typedef struct cs_rawText_queue_struct cs_rawText_queue;
typedef struct cs_rawText_struct cs_rawText_t;

void watcher(uv_idle_t *handle);
size_t save_data(void *ptr, size_t size, size_t nmemb, void *save);
void download(uv_work_t *req);
void work_done(uv_work_t *req, int status);


#endif
