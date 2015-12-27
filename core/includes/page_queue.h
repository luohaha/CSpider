#include "pageProcesser.h"

void page_queue_init(void);

page_queue_id new_page_queue(unsigned int capacity);

void destroy_page_queue(page_queue_id id);

page_id alloc_page_from_queue(page_queue_id queue_id);

free_page_from_queue(page_id pid);

cs_page* get_page_from_id(page_id pid);
