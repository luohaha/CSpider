#include "cs_string.h"

/*
  set_string
*/

int set_string(cs_page* p, char* context) {
  if(p == NULL)
    return 0x1; /* 1 */
  if(context == NULL)
    return 0x10; /* 2 */
    
  unsigned int length = strlen(context);
  if(length == 0)
    return 0x100; /* 3 */
  
  if(p->data == NULL) {
    unsigned int capacity = \
      (length / 64 + (unsigned int)((length % 64) != 0)) * 64; /* floor to 64 bytes */
    void* buf = malloc(capacity);
    if(buf == NULL)
      return 0x1000; /* 4 */
    p->capacity = capacity;
    p->data = buf;
    p->used = 0;
    p->file_type = FileTypeErr;
  }
  else if(p->capacity - p->used < length) {
    unsigned int capacity = p->capacity + \
      (length / 64 + (unsigned int)((length % 64) != 0)) * 64; /* floor to 64 bytes */
    void* buf = malloc(capacity);
    if(buf == NULL)
      return 0x10000; /* 5 */
    memcpy(buf, p->data, p->used);
    free(p->data);
    p->capacity = capacity;
    p->data = buf;
  }
  
  memcpy(p->data + p->used, context, length);
  p->used += length;
  
  return 0;
}
