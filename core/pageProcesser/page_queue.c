
#include <string.h>
#include "cs_page.h"

cs_page_queue private_page_queues[MaxPageQueueNum];
int private_is_queue_used[MaxPageQueueNum];

/*
  page_queue_init: must be called when starts.
*/

void page_queue_init(void) {
  memset(private_page_queues, 0, MaxPageQueueNum * sizeof(cs_page_queue));
  memset(private_is_queue_used, 0, MaxPageQueueNum);
}

/*
  new_page_queue
*/

#define BadPageQueueID ((unsigned int) -1)
typedef unsigned int page_queue_id;

page_queue_id new_page_queue(unsigned int capacity) {
  page_queue_id id = BadPageQueueID;
  int i;
  for(i = 0; i < MaxPageQueueNum; i++) {
    if(private_is_queue_used[i] == FALSE) {
      id = i;
      private_is_queue_used[i] = TRUE;
      break;
    }
  }
  if(id == BadPageQueueID)
    goto label_end;
  cs_page_queue queue = private_page_queues[id];
  queue.pages = (cs_page*) malloc(capacity * sizeof(cs_page));
  queue.capacity = capacity;
  queue.usage = 0;
  private_page_queues[id] = queue;
label_end:
  return id;
}

/*
  destroy_page_queue
*/

void destroy_page_queue(page_queue_id id) {
  cs_page_queue queue = private_page_queues[id];
  int i;
  if(queue.pages != NULL) {
    for(i = 0; i < queue.capacity; i++) {
      void* p = queue.pages[i].data;
      if(p != NULL)
        free(p);
    }
    free(queue.pages);
  }
  memset(&private_page_queues[id], 0, sizeof(cs_page_queue));
  private_is_queue_used[id] = FALSE;
}

/*
  alloc_page_from_queue
*/

#define BadPageID ((unsigned int) -1)
typedef unsigned int page_id;

page_id alloc_page_from_queue(page_queue_id queue_id) {
  page_id pid = BadPageID;
  cs_page_queue queue = private_page_queues[queue_id];
  if(queue.pages == NULL)
    return pid;
  for(int i = 0; i < queue.capacity; i++) {
    if(queue.pages[i].data == NULL) {
      pid = (queue_id << (sizeof(page_id) - LogMaxPageQueueNum)) | queue_id;
      queue.usage ++;
      private_page_queues[queue_id] = queue;
      break;
    }
  }
  //label_end:
  return pid;
}

/*
  free_page_from_queue
*/

void free_page_from_queue(page_id pid) {
  cs_page_queue* p_queue = &private_page_queues[pid >> (sizeof(page_id) - LogMaxPageQueueNum)];
  destroy_page(&p_queue->pages[pid << LogMaxPageQueueNum >> LogMaxPageQueueNum]);
  p_queue->usage --;
}

/*
  get_page_from_id
*/
cs_page* get_page_from_id(page_id pid) {
  return &private_page_queues[pid >> (sizeof(page_id) - LogMaxPageQueueNum)].\
    pages[pid << LogMaxPageQueueNum >> LogMaxPageQueueNum];
}
