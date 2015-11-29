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

/*
  将获取到的数据加入队列
*/
void addData(textq *head, textq *queue) {

  //textq *queue = (textq*)malloc(sizeof(textq));
  //queue->data = data;
  queue->next = head;
  queue->prev = head->prev;
  queue->next->prev = queue;
  queue->prev->next = queue;
  return;
}
/*
  从队列中取出数据
*/
textq *removeData(textq *head, text *data) {
  textq *p = head->next;
  textq *res = NULL;

  while (p != head) {
    if (p->data == data) {
      res = p;
      p->prev->next = p->next;
      p->next->prev = p->prev;
      break;
    } else {
      p = p->next;
    }
  }
  
  return res;
}
/*
回收数据
*/
void freeData(textq *node) {
  free(node->data->worker);
  free(node->data->data);
  free(node->data);
  free(node);
}
