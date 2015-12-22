#ifndef UTILS_H
#define UTILS_H

#include "CS.h"

typedef unsigned int (*hashfunc_t)(char *);
typedef struct Bloom_struct{
  size_t asize; // bloom filter's bit array's size
  unsigned char *a; // bit array
  size_t nfuncs; // hash function's number
  hashfunc_t *funcs; // array of hash function
} Bloom_t;

/*
 bloom filter
*/
#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

Bloom_t *create_bloom(size_t size, size_t nfuncs, ...);
int bloom_destroy(Bloom_t *bloom);
int bloom_add(Bloom_t *bloom, char *s);
int bloom_check(Bloom_t *bloom, char *s);
Bloom_t *init_Bloom();
/*
  regex.c
*/

#define REGEX_ALL 0
#define REGEX_NO_ALL 1

int regexAll(const char *regex, char *str, char **res, int num, int flag);
int match(char *regex, char *str);
//xpath.c
int xpath(char *xml, char *path, char **get, int num);
int  execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, char** get, int num);
int print_xpath_nodes(xmlNodeSetPtr nodes, char **get, int num);

//log.c
void logger(int flag, const char *str1, char *str2, cspider_t *cspider);


/*
  hash functions
*/
unsigned int sax_hash(char *key);
unsigned int sdbm_hash(char *key);

#endif
