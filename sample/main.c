#include<cspider/spider.h>

void p(cspider_t *cspider, char *d) {
  //char *get = regex("http://(.+?).com", d, REGEX_ALL);
  saveString(cspider, d);
  
    //addUrl(cspider, get, NULL, 1);
}

void s(char *str) {
  printf("%s\n", str);
  return;
}

int main() {
  //spider = (cspider_t*)malloc(sizeof(cspider_t));
  cspider_t *spider = init_cspider();
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  char *cookie = "bid=\"s3/yuH5Jd/I\"; _pk_ref.100001.8cb4=%5B%22%22%2C%22%22%2C1447833068%2C%22http%3A%2F%2Fwww.baidu.com%2Flink%3Furl%3D5D2629D7-cg0AVzJBbGSaU7HsFJAnYKzoAa9L68zVOMMQWPFJui-g_GquR_tnSJ9%26wd%3D%26eqid%3Df3e320390001b5dc00000003564c2de3%22%5D; _pk_id.100001.8cb4=8196f325b29ea5c3.1444265431.3.1447833068.1444466917.; ll=\"108288\"; viewed=\"1130500_24708145_6433169_4843567_1767120_5318823_1899158_1271597\"; __utma=30149280.927537245.1446813674.1446983217.1449139583.4; __utmz=30149280.1449139583.4.4.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/login; ps=y; ue=\"965166527@qq.com\"; dbcl2=\"58742090:QgZ2PSLiDLQ\"; ck=\"T9Wn\"; push_noty_num=0; push_doumail_num=7; ap=1; __utmb=30149280.0.10.1449139583; __utmc=30149280";
  //cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_url(spider, "http://accounts.douban.com/login?uid=&alias=18810541851%40163.com&redir=http%3A%2F%2Fmovie.douban.com%2F&source=movie&error=1012", cookie, 1);
  //cs_setopt_url(spider, "www.youku.com", NULL, 1);
  //cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_useragent(spider, agent);
  cs_setopt_process(spider, p);
  cs_setopt_save(spider, s);
  cs_setopt_threadnum(spider, DOWNLOAD, 4);
  cs_setopt_threadnum(spider, SAVE, 1);
  
  return cs_run(spider);
}
