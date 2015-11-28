#include "spider.h"

int main() {
  //spider = (cspider_t*)malloc(sizeof(cspider_t));
  cspider_t *spider = init_cspider();
  cs_setopt_threadnum(spider, DOWNLOAD, 1);
  cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_url(spider, "www.youku.com", NULL, 1);
  return cs_run(spider);
}
