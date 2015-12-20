#include<cspider/spider.h>

void p(cspider_t *cspider, char *d, char *url, void *user_data) {
  printf("url -> %s\n", url);
  saveString(cspider, d);
  
}

void s(void *str, void *user_data) {
  char *get = (char *)str;
  FILE *file = (FILE*)user_data;
  fprintf(file, "%s\n", get);
  return;
}

int main() {
  cspider_t *spider = init_cspider(); 
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  
  cs_setopt_url(spider, "github.com");
  
  cs_setopt_useragent(spider, agent);
  //指向自定义的解析函数，和数据持久化函数
  cs_setopt_process(spider, p, NULL);
  cs_setopt_save(spider, s, stdout);
  //设置抓取线程数量，和数据持久化的线程数量
  cs_setopt_threadnum(spider, DOWNLOAD, 2);
  cs_setopt_threadnum(spider, SAVE, 2);
  
  return cs_run(spider);
}
