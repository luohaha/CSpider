#include "pageProcesser.h"
#define textq cs_rawText_queue
#define text  cs_rawText_t
/*
data.c provide some functions to handle cs_rawText_t, 
such as create, insert, delete and so on.
*/

/**
  isDataQueueEmpty : is data queue empty?
  @head : the data queue ready to be tested

  return 1 for queue empty, and 0 for queue no empty
**/
int isDataQueueEmpty(cs_rawText_queue *head) {
  return (head->next == head);
}
/**

initDataQueue : return initial data queue
**/
textq *initDataQueue() {
  textq *queue = (textq*) malloc(sizeof(textq));
  PANIC(queue);
  
  queue->next = queue;
  queue->prev = queue;
  queue->data = NULL;
  return queue;
}

/**
  createData : create data struct
  
  return the data struct which we create
**/
text *createData() {
  text *node = (text*)malloc(sizeof(text));
  PANIC(node);
  
  node->count = 0;
  node->length = 0;
  
  return node;
}

/**
  addData : add data struct into data queue
  @head : the data queue ready to be inserted
  @queue : will insert into @head
**/
void addData(textq *head, textq *queue) {

  
  queue->next = head;
  queue->prev = head->prev;
  queue->next->prev = queue;
  queue->prev->next = queue;
  return;
}
/**
  
  removeData : remove data from data queue
  @head : the data queue ready to be removed from
  @data : the data struct ready to removed from @head

  if @data exists in @head
  return @data
  else
  return NULL
**/
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
/**

freeData : free data queue
@node : ready to be freed

first, free the worker
second, free the array of buffer
third, free the url
fourth: free data struct
finally, free data queue
**/
void freeData(textq *node) {
  free(node->data->worker);
  int i;
  for (i = 0; i < node->data->count; i++) {
    //逐个free数据块
    //free buffers one by one
    free(node->data->data[i]);
  }
  free(node->data->url);
  free(node->data);
  free(node);
}
