#include "pageProcesser.h"
#define textq cs_rawText_queue
#define text  cs_rawText_t
/*
data.c provide some functions to handle cs_rawText_t, 
such as create, insert, delete and so on.


*/
/*
  判断数据队列是否未空
  is data queue empty?
*/
int isDataQueueEmpty(cs_rawText_queue *head) {
  return (head->next == head);
}
/*
新建头节点

return initial data queue
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

  create data struct
*/
text *createData() {
  text *node = (text*)malloc(sizeof(text));
  
  node->count = 0;
  node->length = 0;
  
  return node;
}

/*
  将获取到的数据加入队列

  add data into data queue
*/
void addData(textq *head, textq *queue) {

  
  queue->next = head;
  queue->prev = head->prev;
  queue->next->prev = queue;
  queue->prev->next = queue;
  return;
}
/*
  从队列中取出数据

  remove data from data queue
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

free data queue
*/
void freeData(textq *node) {
  free(node->data->worker);
  int i;
  for (i = 0; i < node->data->count; i++) {
    //逐个free数据块
    //free buffers one by one
    free(node->data->data[i]);
  }
  
  free(node->data);
  free(node);
}
