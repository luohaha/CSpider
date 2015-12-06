# CSpider


c语言实现的易用的高效的网络爬虫框架:)。

##安装与使用方法：
1. 确认你的电脑中，已经安装下面的库：  
	* curl
	* libuv
	* libxml2
	* pcre
2. 进入`core`文件夹，编译文件，命令如下：  
```
make
```
3. 接着要配置动态库，将`.so`文件和`.h`文件移动到相应的位置，命令如下:  
```
make config
```
4. 这时，就可以开始编译你写好的文件（例如 `test.c`），命令如下:  
```
gcc -o test test.c -lcspider -I /usr/include/libxml2
```

>其中，`-lcspider`将链接我们上述编译完成的cspider动态链接库，而`-I /usr/include/libxml2`则是让编译器能够找到libxml2库的头文件，当然你最好使用Makefile文件。  

##API
* `cspider_t *init_cspider()`  
	在程序一开始的时候，获取到`cspider_t`变量。是必不可少的函数。  
	
* `void cs_setopt_url(cspider_t *, char *)`  
	为`cspider_t`设置程序启动时要抓取的url，在第二个参数传入url字符串即可，url字符串可以不用包含`http://`或`htpps://`。可以多次调用，即可插入多个url任务。
	
* `void cs_setopt_cookie(cspider_t *, char *)`  
	第二个参数设置cookie，格式为`var1=abc; var2=qwe`。可选项。
	
* `void cs_setopt_useragent(cspider_t *, char *)`  
	第二个参数设置user agent，可选项。
	
* `void cs_setopt_proxy(cspider_t *, char *)`  
	第二个参数设置代理的地址，可选项。  
	
* `void cs_setopt_timeout(cspider_t *, long)`  
	设置超时时间，单位为毫秒，可选项。
	
* `void cs_setopt_logfile(cspider_t *, FILE *)`  
	设置输出日志的打印文件，可选项。不设置，则不会输出日志。  
	
* `void cs_setopt_process(cspider_t *, void (*process)(cspider_t *, char*))`
	设置解析函数，解析函数的第二个参数为抓取到的完整的数据字符串。
	
* `void cs_setopt_save(cspider_t *, void (*save)(void*))`  
	设置数据持久化函数。
	
* `void cs_setopt_threadnum(cspider_t *cspider, int , int )`  
	设置线程的数量，其中，第二个参数，可以传入`DOWNLOAD`与`SAVE`，分别表示，将要设置下载线程的数量，或是解析与保存线程的数量。第三个参数为要设置的线程的数量。可选项，默认的线程数量均为5个。  
	
* `int cs_run(cspider_t *)`  
	启动cspider的函数，在程序的最后调用。  
	
* `void saveString(cspider_t *, char *)`  
	在自定义的解析函数中，可以调用此函数。数据持久化，将字符串传入自定义的数据持久化函数。  
	
* `void addUrl(cspider_t *cspider, char *url)`  
	在自定义的解析函数中，调用此函数。可以将解析获取到的url，再次加入任务队列。
	
##使用例子
输出乐视电影页面，第一页的所有电影名称。  

```c
#include<cspider/spider.h>

/*
	自定义的解析函数，d为获取到的html页面字符串
*/
void p(cspider_t *cspider, char *d) {
  
  char *get[100];
  //xpath解析html
  int size = xpath(d, "//body/div[@class='wrap']/div[@class='sort-column area']/div[@class='column-bd cfix']/ul[@class='st-list cfix']/li/strong/a", get);
    
  int i;
  for (i = 0; i < size; i++) {
  //将获取到的电影名称，持久化
    saveString(cspider, get[i]);
  }
  
}
/*
	数据持久化函数，对上面解析函数中调用的saveString()函数传入的数据，进行进一步的保存
*/
void s(void *str) {
  char *get = (char *)str;
  printf("%s\n", get);
  return;
}

int main() {
  //初始化spider
  cspider_t *spider = init_cspider();
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  //char *cookie = "bid=s3/yuH5Jd/I; ll=108288; viewed=1130500_24708145_6433169_4843567_1767120_5318823_1899158_1271597; __utma=30149280.927537245.1446813674.1446983217.1449139583.4; __utmz=30149280.1449139583.4.4.utmcsr=accounts.douban.com|utmccn=(referral)|utmcmd=referral|utmcct=/login; ps=y; ue=965166527@qq.com; dbcl2=58742090:QgZ2PSLiDLQ; ck=T9Wn; push_noty_num=0; push_doumail_num=7; ap=1; __utmb=30149280.0.10.1449139583; __utmc=30149280";
  
  //设置要抓取页面的url
  cs_setopt_url(spider, "so.tv.sohu.com/list_p1100_p20_p3_u5185_u5730_p40_p5_p6_p77_p80_p9_2d1_p101_p11.html");
  //设置user agent
  cs_setopt_useragent(spider, agent);
  //cs_setopt_cookie(spider, cookie);
  //传入解析函数和数据持久化函数的指针
  cs_setopt_process(spider, p);
  cs_setopt_save(spider, s);
  //设置线程数量
  cs_setopt_threadnum(spider, DOWNLOAD, 2);
  cs_setopt_threadnum(spider, SAVE, 2);
  //FILE *fp = fopen("log", "wb+");
  //cs_setopt_logfile(spider, fp);
  //开始爬虫
  return cs_run(spider);
}

```

