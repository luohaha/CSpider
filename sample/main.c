#include<cspider/spider.h>
#include <locale.h>

void p(cspider_t *cspider, char *d, char *url, void *user_data) {
  char *get[10];
  int size = xpath(d, "//a/@href", get, 10);
//  int size = regexAll("http:\\/\\/(.*?)\\.html", d, get, 3, REGEX_ALL);

  joinall(url,get,size);
  addUrls(cspider, get, size);
  saveStrings(cspider, (void**)get, size, LOCK);
  freeStrings(get, size);
}

void s(void *str, void *user_data) {
  char *get = (char*)str;
  FILE *file = (FILE*)user_data;
//  fprintf(file, "%s\n", get);
  return;
}

int main() {
  cspider_t *spider = init_cspider();
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  setlocale(LC_ALL,"");

  cs_setopt_url(spider, "http://www.youku.com/v_olist/c_96_s_0_d_0_g_0_a_0_r_0_u_0_pt_0_av_0_ag_0_sg_0_mt_0_lg_0_q_0_pr_0_h_0_p_1.html");

  cs_setopt_useragent(spider, agent);
  //指向自定义的解析函数，和数据持久化函数
  cs_setopt_process(spider, p, NULL);
  cs_setopt_save(spider, s, stdout);
  //设置抓取线程数量，和数据持久化的线程数量
  cs_setopt_threadnum(spider, DOWNLOAD, 2);
  cs_setopt_threadnum(spider, SAVE, 2);
  cs_setopt_logfile(spider, stdout);

  return cs_run(spider);
}
