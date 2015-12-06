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

int regexAll(const char *regex, char *str, char **res, int num, int flag);
int match(char *regex, char *str);
//xpath.c
int xpath(char *xml, char *path, char **get, int num);
int  execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, char** get, int num);
int print_xpath_nodes(xmlNodeSetPtr nodes, char **get, int num);

#endif
