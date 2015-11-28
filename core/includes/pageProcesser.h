#ifndef PAGEPROCESSER_H
#define PAGEPROCESSER_H

#include "CS.h"

#define REGEX_ALL 0
#define REGEX_NO_ALL 1

char *regex(const char *regex, char *str, int flag);
char **regexAll(const char *regex, char *str, int *num, int flag);

#endif
