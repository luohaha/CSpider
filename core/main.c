#include "spider.h"
#include "pageProcesser.h"

void p(cspider_t *cspider, char *d) {
  saveString(cspider, d);
}

void s(char *str) {
  printf("%s\n", str);
}

int main() {
  //spider = (cspider_t*)malloc(sizeof(cspider_t));
  cspider_t *spider = init_cspider();
  
  cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_url(spider, "www.youku.com", NULL, 1);
  cs_setopt_process(spider, p);
  cs_setopt_save(spider, s);
  
  return cs_run(spider);
}
