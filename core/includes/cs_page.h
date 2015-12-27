#include "pageProcesser.h"

void clear_page(cs_page *p);
void destroy_page(cs_page *p);
int new_page(cs_page *p, unsigned int capacity);
int set_page(cs_page *p, char* context, unsigned int length);

