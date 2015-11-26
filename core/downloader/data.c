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
text *createData(const char* type) {
  text *node = (text*)malloc(sizeof(text));
  char *buf = (char*)malloc(sizeof(char));
  node->data = buf;
  node->type = type;
  
  return node;
}

