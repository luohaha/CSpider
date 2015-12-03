#ifndef UTILS_H
#define UTILS_H

#include "CS.h"

/*
key value对
*/
typedef struct kv_struct {
  char *key;
  char *value;
} kv_t;

/*
正则表达式
*/
#define REGEX_ALL 0
#define REGEX_NO_ALL 1

char *regex(const char *regex, char *str, int flag);


#endif
