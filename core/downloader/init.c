#include "downloader.h"
#define textq cs_rawText_queue
#define text  cs_rawText_t
/*
新建头节点
*/
textq *initDataQueue() {
  textq *queue = (textq*) malloc(sizeof(textq));
  queue->next = queue;
  queue->prev = queue;
  queue->data = NULL;
  return queue;
}

/*
  创建节点
*/
void createDataAndInsert(textq *head, char *begin, const char* type) {
  text *node = (text*)malloc(sizeof(text));
  node->data = begin;
  node->type = type;
  textq *link = (textq*)malloc(sizeof(textq));
  link->data = node;
  link->prev = head->prev;
  link->next = head;
  link->prev->next = link;
  link->next->prev = link;
  return;
}
