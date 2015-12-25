#include <string.h>
#include "pageProcesser.h"

void clear_page(cs_page *p);
void destroy_page(cs_page *p);
int new_page(cs_page *p, unsigned int capacity);
int set_page(cs_page *p, char* context, unsigned int length);

/*
  clear_page: fill the data with all zeros.
*/

void clear_page(cs_page *p) {
  if(p == NULL)
    return;
  if(p->data == NULL)
    return;
  memset(p->data, 0, p->capacity);
  p->used = 0;
  p->file_type = FileTypeErr;
}

/*
  destroy_page: recycle the memory. Optionally before new_page or set_page.
*/

void destroy_page(cs_page *p) {
  if(p == NULL)
    return;
  if(p->data != NULL)
    free(p->data);
  memset(p, 0, sizeof(cs_page));
}

/*
  new_page: pre-allocate the memory before using, optionally.
*/

int new_page(cs_page *p, unsigned int capacity) {
  if(p == NULL)
    return 0x1;
  if(capacity == 0)
    return 0x10;
  void* buf = malloc(capacity);
  if(buf == NULL)
    return 0x100;
  p->data = buf;
  p->capacity = capacity;
  memset(p->data, 0, p->capacity);
  /* set p->file_type and p->used to zeros */
  memset(&p->file_type, 0, sizeof(cs_page) - &0->file_type);
}

/*
  set_page: remember to set the file_type manually.
*/

int set_page(cs_page *p, char* context, unsigned int length) {
  if(p == NULL)
    return 0x1;
  if(context == NULL)
    return 0x10;
  if(p->length == 0)
    return 0x100;
  if(p->data == NULL) {
    free(p->data);
    goto label_next;
  }
  if(p.capacity < length) {
label_next:
    unsigned int capacity = (length / 512 + (unsigned int)(length % 512 != 0)) * 512; /* floor to 512 bytes */
    void* buf = malloc(capacity);
    if(buf == NULL)
      return 0x1000;
    p->capacity = capacity;
    p->data = buf;
  }
  p->used = length
  p->file_type = FileTypeErr;
  memcpy(p->data, context, length);
  
  return 0;
}
