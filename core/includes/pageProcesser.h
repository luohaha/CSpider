#ifndef PAGEPROCESSER_H_DEF
#define PAGEPROCESSER_H_DEF

#include "CS.h"
#include "spider.h"
#include "downloader.h"

/*

*/
#define LOCK 1
#define NO_LOCK 0
/*
  raw data, such as html and json which we get.
*/
struct cs_rawText_struct {
  char *data[BUFFER_MAX_NUMBER]; /* Array of buffer */
  unsigned int each[BUFFER_MAX_NUMBER]; /* each buffer's size */
  int count;/* buffer's number */
  int length;/* the sum of all buffer's size */
  char *url; /* the url where it is downloaded */
  uv_work_t *worker;/* Point to the worker */
  cspider_t *cspider;/* the Main cspider struct */
};


/*------------------------------------------------------------
  Added since Dec. 25/2015, don't touch it until finished.  
  To find more information in page_processer.c .
-------------------------block start-------------------------*/

/*
  page carrier
*/
#define FileTypeErr 0
#define FileTypeHTML 1
#define FileTypeCSS 2
#define FileTypeJSON 3

struct cs_page_struct {
  void *data;
  unsigned int capacity;
  unsigned int used;
  char file_type;
};

/*
  page carrier'a queue
*/
#define MaxPageQueueNum 32
#define LogMaxPageQueueNum 5

struct cs_page_queue_struct {
  cs_page *pages;
  unsigned int capacity;
  unsigned int usage;
};

/*-------------------------block end-------------------------*/

/*
 data queue
*/
struct cs_rawText_queue_struct {
  cs_rawText_t *data;
  struct cs_rawText_queue_struct *next; /* next node */
  struct cs_rawText_queue_struct *prev; /* previous node */
};

/*data.c*/
int isDataQueueEmpty(cs_rawText_queue *head);
cs_rawText_queue *initDataQueue();
cs_rawText_t *createData();
void addData(cs_rawText_queue *head, cs_rawText_queue *queue);
cs_rawText_queue *removeData(cs_rawText_queue *head, cs_rawText_t *data);
void freeData(cs_rawText_queue *node);


void dataproc(uv_work_t *req);
void datasave(uv_work_t *req, int status);
/*
  user interface
*/
void saveString(cspider_t *cspider, void *data, int flag);
void saveStrings(cspider_t *cspider, void **datas, int size, int flag);
void addUrl(cspider_t *cspider, char *url);
void addUrls(cspider_t *cspider, char **urls, int size);
void freeString(char *str);
void freeStrings(char **strs, int size);

#endif
